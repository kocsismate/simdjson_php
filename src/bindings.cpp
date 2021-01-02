/*
  +----------------------------------------------------------------------+
  | simdjson_php                                                         |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  +----------------------------------------------------------------------+
  | Author: Jinxi Wang  <1054636713@qq.com>                              |
  | Author: Máté Kocsis <kocsismate@woohoolabs.com>                      |
  +----------------------------------------------------------------------+
*/

#include "php.h"
#include "php_simdjson.h"

#include "simdjson.h"
#include "bindings.h"

extern "C" {
	#include <ext/json/php_json.h>
	#include <Zend/zend_exceptions.h>
	#include <Zend/zend_object_handlers.h>
}

extern PHP_JSON_API zend_class_entry *php_json_exception_ce;

#if PHP_VERSION_ID < 80000
extern ZEND_API zval *zend_std_write_property(zval *object, zend_string *member, zval *value, void **cache_slot);
#else
extern ZEND_API zval *zend_std_write_property(zend_object *object, zval *member, zval *value, void **cache_slot);
#endif

// see https://github.com/simdjson/simdjson/blob/master/doc/performance.md#reusing-the-parser-for-maximum-efficiency
simdjson::dom::parser parser;

WARN_UNUSED
simdjson::error_code
build_parsed_json_cust(
	simdjson::dom::element &doc, const char *buf, size_t len, bool realloc_if_needed, u_short depth = simdjson::DEFAULT_MAX_DEPTH
) {
    auto error = parser.allocate(len, depth);

    if (error) {
        return error;
    }

    error = parser.parse(buf, len, realloc_if_needed).get(doc);
    if (error) {
        return error;
    }

    return simdjson::SUCCESS;
}

static zval create_array(simdjson::dom::element element)
{
    zval v;

    switch (element.type()) {
        case simdjson::dom::element_type::STRING :
        	{
				std::string_view str(element);
            	ZVAL_STRINGL(&v, str.data(), str.length());
            }
            break;
        case simdjson::dom::element_type::INT64 : ZVAL_LONG(&v, int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 : ZVAL_LONG(&v, uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY :
            array_init_size(&v, simdjson::dom::array(element).size());

            for (simdjson::dom::element child : simdjson::dom::array(element)) {
                zval value = create_array(child);
                add_next_index_zval(&v, &value);
            }

            break;
        case simdjson::dom::element_type::OBJECT :
            array_init(&v);

            for (simdjson::dom::object::iterator it = simdjson::dom::object(element).begin(); it != simdjson::dom::object(element).end(); ++it) {
                zval value = create_array(it.value());
                add_assoc_zval_ex(&v, it.key_c_str(), it.key_length(), &value);
            }
            break;
        default:
            break;
    }

    return v;
}

static zval create_object(simdjson::dom::element element)
{
    zval v;

    switch (element.type()) {
        case simdjson::dom::element_type::STRING :
        	{
				std::string_view str(element);
				ZVAL_STRINGL(&v, str.data(), str.length());
			}
            break;
        case simdjson::dom::element_type::INT64 : ZVAL_LONG(&v, int64_t(element));
            break;
        case simdjson::dom::element_type::UINT64 : ZVAL_LONG(&v, uint64_t(element));
            break;
        case simdjson::dom::element_type::DOUBLE : ZVAL_DOUBLE(&v, double(element));
            break;
        case simdjson::dom::element_type::BOOL :
            ZVAL_BOOL(&v, bool(element));
            break;
        case simdjson::dom::element_type::NULL_VALUE :
            ZVAL_NULL(&v);
            break;
        case simdjson::dom::element_type::ARRAY :
            array_init(&v);

            for (simdjson::dom::element child : simdjson::dom::array(element)) {
                zval value = create_object(child);
                add_next_index_zval(&v, &value);
            }
            break;
        case simdjson::dom::element_type::OBJECT :
            object_init(&v);

            for (simdjson::dom::key_value_pair field : simdjson::dom::object(element)) {
                zval value = create_object(field.value);
#if PHP_VERSION_ID < 80000
                zend_std_write_property(&v, zend_string_init(field.key.data(), strlen(field.key.data()), 0), &value, NULL);
#else
				zend_std_write_property(Z_OBJ(v), zend_string_init(field.key.data(), strlen(field.key.data()), 0), &value, NULL);
#endif
            }
            break;
        default:
            break;
    }
    return v;
}

bool cplus_simdjson_is_valid(const char *json, size_t len)
{
    simdjson::dom::element doc;
    auto error = parser.parse(json, len).get(doc);
    if (error) {
        return false;
    }
    return true;
}

void cplus_simdjson_parse(const char *json, size_t len, zval *return_value, zend_bool assoc, zend_long depth)
{
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_error(php_json_exception_ce, simdjson::error_message(error), 0);
        RETURN_THROWS();
    }

    if (assoc) {
        *return_value = create_array(doc);
    } else {
        *return_value = create_object(doc);
    }
}

void cplus_simdjson_key_value(
	const char *json, size_t len, const char *key, zval *return_value, zend_bool assoc, zend_long depth
) {
    simdjson::dom::element doc;
    simdjson::dom::element element;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_error(php_json_exception_ce, simdjson::error_message(error), 0);
        RETURN_THROWS();
    }

    error = doc.at(key).get(element);

    if (error) {
        zend_throw_error(php_json_exception_ce, simdjson::error_message(error), 0);
        RETURN_THROWS();
    }

    if (assoc) {
        *return_value = create_array(element);
    } else {
        *return_value = create_object(element);
    }
}

u_short cplus_simdjson_key_exists(const char *json, size_t len, const char *key, zend_long depth)
{
    simdjson::dom::element doc;
    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }
    error = doc.at(key).error();
    if (error) {
        return SIMDJSON_PARSE_KEY_NOEXISTS;
    }
    return SIMDJSON_PARSE_KEY_EXISTS;
}

void cplus_simdjson_key_count(const char *json, size_t len, const char *key, zval *return_value, zend_long depth)
{
    simdjson::dom::element doc;
    simdjson::dom::element element;

    auto error = build_parsed_json_cust(doc, json, len, true, depth);
    if (error) {
        zend_throw_error(php_json_exception_ce, simdjson::error_message(error), 0);
        RETURN_THROWS();
    }

    error = doc.at(key).get(element);
    if (error) {
        zend_throw_error(php_json_exception_ce, simdjson::error_message(error), 0);
        RETURN_THROWS();
    }

    switch (element.type()) {
        case simdjson::dom::element_type::ARRAY : ZVAL_LONG(return_value, uint64_t(simdjson::dom::array(element).size()));
            break;
        case simdjson::dom::element_type::OBJECT : ZVAL_LONG(return_value, uint64_t(simdjson::dom::object(element).size()));
            break;
        default: ZVAL_LONG(return_value, 0);
            break;
    }
}
