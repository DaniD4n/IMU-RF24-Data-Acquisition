#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

// NRF SETUP =====
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

//Registros MPU6050 (endereço ususal) ====
#define MPU_ADDR 0x68

//STRUCT (data psck)========
struct __attribute__((packed)) IMUData {
  uint32_t timestamp;
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

//===== READ MPU6050 (usando ponteiro pra nn criar cópia)
void readIMU(IMUData* data) { //talvez trocar pra referencia no futuro)

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // ACCEL_XOUT_H
  Wire.endTransmission(false); //cotinuo
  Wire.requestFrom(MPU_ADDR, 14, true);

  data->ax = (Wire.read() << 8) | Wire.read();
  data->ay = (Wire.read() << 8) | Wire.read();
  data->az = (Wire.read() << 8) | Wire.read();

  Wire.read(); Wire.read(); // skip temperature

  data->gx = (Wire.read() << 8) | Wire.read();
  data->gy = (Wire.read() << 8) | Wire.read();
  data->gz = (Wire.read() << 8) | Wire.read();
}


void setup() {

  Wire.begin();
  Wire.setClock(400000); // I2C 400kHz

  Serial.begin(115200); //baud

  // Wake up MPU6050 (talvez modificar para registrtadores melhores)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // eapecifico do MP6050
  Wire.write(0); // acorda e coloca clock padrão
  Wire.endTransmission(true); // stop pra inicialização

  // NRF init
  radio.begin();
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("TX Ready");
}



//SAMPLE CONTROL ===
const uint32_t Ts = 4000; //4000 us = 250 Hz
uint32_t last_time = 0;

void loop() {
  // loop a tempo fixo
  uint32_t now = micros();
  if (now - last_time >= Ts) {

    last_time += Ts;
    IMUData data;
    data.timestamp = now;

    readIMU(&data);

    radio.write(&data, sizeof(data)); 
    println(data) // <- apenas para debugging
  }
}
