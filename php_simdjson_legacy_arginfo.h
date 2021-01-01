/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 9e9495da5ddf842ca6467f0f6d934e771d1ea5d4 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_JsonParser_parse, 0, 0, 1)
	ZEND_ARG_INFO(0, json)
	ZEND_ARG_INFO(0, associative)
	ZEND_ARG_INFO(0, depth)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_JsonParser_isValid, 0, 0, 1)
	ZEND_ARG_INFO(0, json)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_JsonParser_getKeyValue, 0, 0, 2)
	ZEND_ARG_INFO(0, json)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, associative)
	ZEND_ARG_INFO(0, depth)
	ZEND_ARG_INFO(0, flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_JsonParser_getKeyCount, 0, 0, 2)
	ZEND_ARG_INFO(0, json)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()

#define arginfo_class_JsonParser_keyExists arginfo_class_JsonParser_getKeyCount

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_JsonEncoder_encode, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, flags)
	ZEND_ARG_INFO(0, depth)
ZEND_END_ARG_INFO()


ZEND_METHOD(JsonParser, parse);
ZEND_METHOD(JsonParser, isValid);
ZEND_METHOD(JsonParser, getKeyValue);
ZEND_METHOD(JsonParser, getKeyCount);
ZEND_METHOD(JsonParser, keyExists);
ZEND_METHOD(JsonEncoder, encode);


static const zend_function_entry class_JsonParser_methods[] = {
	ZEND_ME(JsonParser, parse, arginfo_class_JsonParser_parse, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(JsonParser, isValid, arginfo_class_JsonParser_isValid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(JsonParser, getKeyValue, arginfo_class_JsonParser_getKeyValue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(JsonParser, getKeyCount, arginfo_class_JsonParser_getKeyCount, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(JsonParser, keyExists, arginfo_class_JsonParser_keyExists, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};


static const zend_function_entry class_JsonEncoder_methods[] = {
	ZEND_ME(JsonEncoder, encode, arginfo_class_JsonEncoder_encode, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};
