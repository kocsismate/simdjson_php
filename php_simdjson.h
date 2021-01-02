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
  +----------------------------------------------------------------------+
*/

#ifndef PHP_SIMDJSON_H
#define PHP_SIMDJSON_H

extern zend_module_entry simdjson_module_entry;

#define PHP_SIMDJSON_VERSION                  "0.1.0"
#define SIMDJSON_PARSE_FAIL                   0
#define SIMDJSON_PARSE_SUCCESS                1
#define SIMDJSON_PARSE_KEY_EXISTS             2
#define SIMDJSON_PARSE_KEY_NOEXISTS           3

#define SIMDJSON_PARSE_DEFAULT_DEPTH          512

#define SIMDJSON_RESOUCE_PJH_TYPE             3
#define SIMDJSON_RESOUCE_PJ_TYPE              4

extern PHPAPI void php_var_dump(zval **struc, int level);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level);

#if PHP_VERSION_ID < 80000
#define VALUE_ERROR(n, name, msg) php_error_docref(NULL, E_WARNING, "Argument #%d (%s) %s", n, name, msg); RETURN_FALSE
#define RETURN_THROWS() return
#else
#define VALUE_ERROR(n, name, msg) zend_argument_value_error(n, msg); RETURN_THROWS()
#endif

ZEND_BEGIN_MODULE_GLOBALS(simdjson)
ZEND_END_MODULE_GLOBALS(simdjson)

PHP_MINIT_FUNCTION(simdjson);
PHP_MSHUTDOWN_FUNCTION(simdjson);
PHP_RINIT_FUNCTION(simdjson);
PHP_RSHUTDOWN_FUNCTION(simdjson);
PHP_MINFO_FUNCTION(simdjson);

#endif
