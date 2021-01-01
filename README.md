# simdjson-php

PHP binding for the [simdjson project](https://github.com/lemire/simdjson).

## Requirements

- PHP 7.0+
- We support platforms like Linux or macOS
- Prerequisites: g++ (version 7 or better) or clang++ (version 6 or better), and a 64-bit system with a command-line shell (e.g., Linux, macOS, freeBSD). We also support programming environnements like Visual Studio and Xcode, but different steps are needed

## Compile

```
$ phpize
$ ./configure
$ make
$ make test
$ make install
```

Add the following line to your php.ini

```
extension=simdjson.so
```

## Usage

```php
//Check if a JSON string is valid:
$isValid = JsonParser::isValid($jsonString); //return bool

//Parsing a JSON string. similar to the json_decode() function but without the fourth argument
$parsedJSON = JsonParser::parse($jsonString, true, 512); //return array|object|null. "null" string is not a standard json

/*
{
  "Image": {
    "Width":  800,
    "Height": 600,
    "Title":  "View from 15th Floor",
    "Thumbnail": {
      "Url":    "http://www.example.com/image/481989943",
      "Height": 125,
      "Width":  100
    },
    "Animated" : false,
    "IDs": [116, 943, 234, 38793, {"p": "30"}]
  }
}
*/

//note. "/" is a separator. Can be used as the "key" of the object and the "index" of the array
//E.g. "Image/Thumbnail/Url" is ok.


//get the value of a "key" in a json string
$value = JsonParser::getKeyValue($jsonString, "Image/Thumbnail/Url");
var_dump($value); // string(38) "http://www.example.com/image/481989943"

$value = JsonParser::getKeyValue($jsonString, "Image/IDs/4", true);
var_dump($value);
/*
array(1) {
  ["p"]=>
  string(2) "30"
}
*/

//check if the key exists. return true|false|null. "true" exists, "false" does not exist, "null" string is not a standard json
$res = JsonParser::keyExists($jsonString, "Image/IDs/1");
var_dump($res); //bool(true)

// count the values
$res = JsonParser::getKeyCount($jsonString, "Image/IDs");
var_dump($res); //int(5)
```

## Benchmarks
See the [benchmark](./benchmark) folder for more information.
