Arduino NVS Library
==========================

## Summary
Arduino NVS is a port for a non-volatile storage (NVS, flash) [library](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/nvs_flash.html) for ESP32 to the Arduino Platform. It wraps main NVS functionality into the Arduino-styled C++ class.
This lib is inspired and based on [TridentTD_ESP32NVS](https://github.com/TridentTD/TridentTD_ESP32NVS)  work. This lib is a further development of it, contains a lot of improvements, bugfixes and translation (original text was published on [Thai](https://en.wikipedia.org/wiki/Thai_language)).

# WARNIG: This lib is under active development. Use with CARE! #

## Introduction
NVS lib (commonly mentioned as *"flash lib"*) is a library used for storing data  values in the flash memory in the ESP32. Data are stored in a non-volatile manner, so it is remaining in the memory after power-out or reboot of the ESP32.

The ESP32 NVS stored data in the form of key-value. Keys are ASCII strings, up to 15 characters. Values can have one of the following types:

- integer types: `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32_t`, `int32_t`, `uint64_t`, `int64_t`
- zero-terminated string
- variable length binary data (blob)

Refer to the NVS ESP32 lib [original documentation](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/nvs_flash.html#internals) for a details about internal NVS lib organization.




## Usage

Please include the library.

` #include <ArduinoNvs.h>` 

Then start the NVS every time with the command.

`NVS.begin();` 

  

### Interger, Float, String, Char Array

To store variable data Can be stored on the NVS by variable type.

```c++
NVS.setInt ("myInt", 23); // Stores the integer value 23 into the key named "myInt" on the NVS.
NVS.setFloat ("myFloat", 94.534); // store the real number 94.534 into the key named "myFloat" on the NVS.

String data1 = "Hello String";
NVS.setString ("myString", data); // store the data value into the key named "myString" on the NVS.

const char * data2 = "Hello Char Array";
NVS.setCharArray ("myChArr", data2); // store the data2 value into the key named "myCharArray" on the NVS.

```


NVS retrieval can be run easily.

```c++
int i = NVS.getInt ("myInt"); // Read the value of the key "myInt" on the NVS to store the variable i.
float f = NVS.getFloat ("myFloat"); // Read the value of the "myFloat" key on the NVS to store the variable f
String str = NVS.getString ("myString"); // Read the value of the "myString" key on the NVS stored in the variable str.
char * c_str = NVS.getCharArray ("myChArr"); // read the value of the key "myChArr" on the NVS to store the variable c_str
```


### Object ###

Storing outside variables other than the basic variable types above.
Storing an array of integers, storing struct data, etc.
Can be stored. Use the object example command.

```c++
uint8_t mac [6] = {0xDF, 0xEE, 0x10, 0x49, 0xA1, 0x42};
NVS.setObject ("MAC", & mac, sizeof (mac)); // store mac [6] to key "MAC" on NVS
```

How to retrieve object information saved on the NVS

```c++
uint8_t * mymac;
mymac = (uint8_t *) NVS.getObject ("MAC"); // Read the key value "MAC" on the NVS to store the variable mymac.

Serial.printf ("mac:% 02X:% 02X:% 02X:% 02X:% 02X:% 02X \ n",
               mymac [1], mymac [2], mymac [3], mymac [4], mymac [5]);
               
```

### Main flaws ###
1. **FIXED** ~~Bug. Dynamic memory allocated without freeing~~
1. Check return for EVERY nvs call, that may return Error
1. All key-values are stored in the same namespace - need to give user change this
1. Mandatory commiting after every set - nned to give a user freedom to choose and make `commit()` public
1. Rewrite using `SimpleAsert` lib
1. Wrap low-level ESP32 NVS API into separate class, to make the library more abstract and  be able to ported to the platforms other than ESP32.
