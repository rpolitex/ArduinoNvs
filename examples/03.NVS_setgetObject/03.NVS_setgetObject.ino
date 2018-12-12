
#include <ArduinoNvs.h>

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.println("[ESP32] NVS start");
  NVS.begin();
  
  Serial.println("[NVS] set object to NVS");
  uint8_t mac[6] = {0xDF, 0xEE, 0x10, 0x49, 0xA1, 0x42};
  NVS.setObject( "MAC", &mac, sizeof(mac) );  // จัดเก็บค่า mac[6] ลงที่ key ชื่อ "MAC" บน NVS
  

  Serial.println("[NVS] get object from NVS");
  uint8_t* mymac;
  mymac = (uint8_t*) NVS.getObject( "MAC");   // อ่านค่า key ชื่อ "MAC" บน NVS มาเก็บไว้ที่ตัวแปร mymac
  
  Serial.printf("mac : %02X:%02X:%02X:%02X:%02X:%02X\n", 
                 mymac[0],mymac[1],mymac[2],mymac[3],mymac[4],mymac[5]);

}

void loop() {
  delay(1);
}