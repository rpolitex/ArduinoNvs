
#include <ArduinoNvs.h>

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.println("[ESP32] NVS start");
  NVS.begin();
  
// NVS.eraseAll();      // erase all keys in NVS
// NVS.erase("SSID");   // erase only one key "SSID" in NVS

  String  ssid        = "My Router Name"; 
  String  password    = "The Password of SSID";
  NVS.setString("SSID", ssid);
  NVS.setString("PASSWORD", password);

  char*   note        = "This is a note.";
  NVS.setCharArray("NOTE", note);

  int16_t distance    = 245;    // uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t , int  type able to use
  NVS.setInt("DISTANCE", distance);

  float   temperature = 89.13582;
  NVS.setFloat("TEMPERATURE", temperature);
}

void loop() {
  delay(1);
}
