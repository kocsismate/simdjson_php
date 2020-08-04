--TEST--
simdjson_key_count throws exception if key was not found test

--SKIPIF--
<?php ?>

--FILE--
<?php
$json = file_get_contents(__DIR__ . DIRECTORY_SEPARATOR . '_files' . DIRECTORY_SEPARATOR . 'result.json');

try {
    \simdjson_key_count($json, "unknown", true);
} catch (\RuntimeException $e) {
    var_dump($e->getMessage());
}

?>
--EXPECTF--
string(56) "The JSON field referenced does not exist in this object."