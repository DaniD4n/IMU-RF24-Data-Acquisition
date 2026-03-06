#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

// NRF SETUP
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

// recriando a struct
struct __attribute__((packed)) IMUData {
  uint32_t timestamp;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

void setup(){
  Serial.begin(115200);
  Serial.println("recebendo");

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();

};

void loop(){
  if(radio.available()){
    IMUData data;
    radio.read(&data, sizeof(data));

     // Header
    Serial.write(0xAA);
    Serial.write(0x55);

    // Envia a struct inteira em binário
    Serial.write((uint8_t*)&data, sizeof(data));
  

  };

}
