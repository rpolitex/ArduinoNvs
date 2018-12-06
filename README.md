NVS-ESP32 Library
==========================

## Summary
NVS-ESP32 is a port for a non-volatile storage (NVS, flash) [library](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/nvs_flash.html) for ESP32 to the Arduino Platform. It wraps main NVS functionality into the Arduino-styled C++ class.
This lib is inspired and based on [TridentTD_ESP32NVS](https://github.com/TridentTD/TridentTD_ESP32NVS) 
work. This lib is a further development of it, contains a lot of improvemetns, bugfixes and translation (original text was published on [Thai](https://en.wikipedia.org/wiki/Thai_language)).

## NVS lib
NVS lib (commonly "flash lib") is a library used For storing variable values in non-volatile storage (NVS) space in ESP32.

In the form of key, value

Even with the Reboot, the ESP32 device still recognizes the value.
  
NVS is similar to EEPROM storage but does not require an address.
It is more convenient to use than EEPROM storage.
It is stored in key & value format directly.
  
  
## Help (Usage)

Please include the library.

`` `
#include <TridentTD_ESP32NVS.h>
`` `
  
Then start the NVS every time with the command.
  
`` `
NVS.begin ();
`` `
  
  
  
### Interger, Float, String, Char Array

To store variable data Can be stored on the NVS by variable type.
  
`` `
NVS.setInt ("myInt", 23); // Stores the integer value 23 into the key named "myInt" on the NVS.
NVS.setFloat ("myFloat", 94.534); // store the real number 94.534 into the key named "myFloat" on the NVS.

String data1 = "Hello String";
NVS.setString ("myString", data); // store the data value into the key named "myString" on the NVS.

const char * data2 = "Hello Char Array";
NVS.setCharArray ("myChArr", data2); // store the data2 value into the key named "myCharArray" on the NVS.
`` `
  
NVS retrieval can be run easily.
  
`` `
int i = NVS.getInt ("myInt"); // Read the value of the key "myInt" on the NVS to store the variable i.
float f = NVS.getFloat ("myFloat"); // Read the value of the "myFloat" key on the NVS to store the variable f
String str = NVS.getString ("myString"); // Read the value of the "myString" key on the NVS stored in the variable str.
char * c_str = NVS.getCharArray ("myChArr"); // read the value of the key "myChArr" on the NVS to store the variable c_str
`` `
  
  
Object ###
  
Storing outside variables other than the basic variable types above.
Storing an array of integers, storing struct data, etc.
Can be stored. Use the object example command.
  
`` `
uint8_t mac [6] = {0xDF, 0xEE, 0x10, 0x49, 0xA1, 0x42};
NVS.setObject ("MAC", & mac, sizeof (mac)); // store mac [6] to key "MAC" on NVS
`` `
  
How to retrieve object information saved on the NVS
  
`` `
uint8_t * mymac;
mymac = (uint8_t *) NVS.getObject ("MAC"); // Read the key value "MAC" on the NVS to store the variable mymac.

Serial.printf ("mac:% 02X:% 02X:% 02X:% 02X:% 02X:% 02X \ n",
               mymac [1], mymac [2], mymac [3], mymac [4], mymac [5]);
               
`` `
  
  
  
Phiên bản
-----
1.0 TridentTD
21 November 2560 Buddism Era (2017)
