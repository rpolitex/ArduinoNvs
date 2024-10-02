#include <Arduino.h>
#include "ArduinoNvs.h"

#include <inttypes.h>
#include <stdio.h>

/** 
 * Arduino NVS is a port for a non-volatile storage (NVS, flash) library for ESP32 to the Arduino Platform. 
 * It wraps main NVS functionality into the Arduino-styled C++ class. 
 */

bool res;

void setup() {
    String _namespace = "storage";
    NVS.begin(_namespace);

    
    delay(5000);
    printf( "\n\n\nNVS opened namespace 'storage'." );
    uint8_t access_count = NVS.getInt("access_count");
    printf( " access_count: %d", access_count );
    res = NVS.setInt("access_count", access_count+1 );
    if( res )
        printf( " .. updated access_count!\n\n" );

    uint8_t entries = NVS.stats();
    printf( " .. found %d entries\n", entries );

    /*** Int ***/
    printf( "Writing integers will choose the appropriate format (16-bit, 32-bit, 64-bit, signed/unsigned):\n" );
    const int16_t i16_data = -6203;
    res = NVS.setInt("data_i16", i16_data);
    if( res ){
        int16_t data = NVS.getInt("data_i16");
        printf( " .. read back written key 'data_i16': %d\n\n", data );
    }

    const uint64_t ui64_data = 0x123456789ABCDEF0ULL;
    res = NVS.setInt("data_u64", ui64_data);
    if( res ){
        uint64_t data = NVS.getInt("data_u64");    
        printf( " .. read back written key 'data_u64': 0x%016" PRIx64 "\n\n", data );
    }

    /*** String ***/
    const String msg_str = "Fils test message";
    res = NVS.setString("message", msg_str);
    if( res ){
        String str = NVS.getString("message");
        printf( " .. read back message: '%s'\n\n", str.c_str() );
    }
    
    /*** BLOB ***/
    uint8_t mac[6] = {0xDF, 0xEE, 0x10, 0x49, 0xA1, 0x42};
    printf( "Writing mac-address block: [DF:EE:10:49:A1:42].\n" );
    if( store.setBlob("mac_addr", mac, sizeof(mac)) ){
        size_t len = store.getBlobSize("mac_addr");
        uint8_t data[20] = {0};
        printf( " .. 'mac_addr' is %d bytes long.", len );

        size_t len2 = store.getBlob("mac_addr", data);
        printf( " .. read back mac (%d/%d bytes) : [%02X:%02X:%02X:%02X:%02X:%02X].\n\n", 
                len,len2, data[0],data[1],data[2],data[3],data[4],data[5] );

        // now reading same contents but with a std::vector
        std::vector<uint8_t> vector_data = store.getBlob("mac_addr");
        printf( "Vector contents (mac_addr): " );
        for (uint8_t byte : vector_data) 
            printf("%02X:", byte);
        printf( "\n\n" );

        // now saving data as a vector, then re-reading as a vector
        if( store.setBlob("vec-data", vector_data) ){
            std::vector<uint8_t> data6 = store.getBlob("vec-data");
            printf( "Vector contents (vec-data): " );
            for (uint8_t byte : data6) 
                printf("%02X:", byte);
            printf( "\n\n" );
        }
    }

    // here we write the data by passing a set of it in directly
    if( store.setBlob("data_block", { 0x99, 0x11, 0x66 }) ){
        size_t len = store.getBlobSize("data_block");
        uint8_t data[20] = {0};
        printf( " .. wrote [99:11:66], read back %d bytes", len );
        len = store.getBlob("data_block", data);
        printf( " .. read back (%d bytes): [%02X:%02X:%02X].\n", len, data[0], data[1], data[2] );
    }
    
    std::vector<String> matches = NVS.entryFind();
    printf("Entries starting with 'data_': ");
    for (const String& match : matches) {
        printf( " - %s\n", match.c_str());
    }


    if( NVS.exists("data_set") ){
        printf( " .. checked and found 'data_set' exists!\n\n" );
    }

    if( !NVS.exists("boomboom") ){
        printf( " .. checked for non-existant key, and it was not found!\n\n" );
    }

    res = NVS.erase("inline_data");
    if( res ){
        printf( " .. deleted 'inline_data' data!\n\n" );
    }

    printf( "That's all folks!\n\n" );
    delay(10000);

}


void loop() {	
    
}
