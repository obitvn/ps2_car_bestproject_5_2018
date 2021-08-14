#include <SPI.h>
#include "RF24.h"
 
const uint64_t pipe = 0xE8E8F0F0E1; // địa chỉ để phát
RF24 radio(9,10); //thay 10 thành 53 với mega
byte msg[3];
const int sensor = A0;
int value = 0;
 
void setup(){ 
  //============================================================Module NRF24
  radio.begin();                     
  radio.setAutoAck(1);               
  radio.setRetries(1,1);             
  radio.setDataRate(RF24_1MBPS);    // Tốc độ truyền
  radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
  radio.setChannel(10);               // Đặt kênh
  radio.openWritingPipe(pipe);        // mở kênh
  pinMode(sensor, INPUT);
}
 
void loop(){
  value = analogRead(sensor);
  msg[0] = value / 4;
  radio.write(&msg, sizeof(msg));
  delay(50);
}
