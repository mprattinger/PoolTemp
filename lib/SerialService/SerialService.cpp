#include <Arduino.h>

#define BAUDRATE 9600

void initSerial(){
    Serial.begin(BAUDRATE);
    while(!Serial){
        ;
    }

    Serial.println();
    Serial.println("Connected! Welcome to FlintSoft Pool Device");
}