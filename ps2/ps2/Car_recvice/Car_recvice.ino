#include <TIMER1_SERVO.h>
#include "RF24.h"
const uint64_t pipe = 0x73AC2D9FB5LL; // địa chỉ phát
RF24 radio(8,10);//thay 10 thành 53 với mega
byte msg[20];
const int led = 3;
int led_st = 0, test=0;
unsigned char goc=90;


void setup(){
port_attach(PB1);// chọn pin ~9

pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
digitalWrite(4, LOW);
msg[17] = 123;
msg[18] = 123;
  // khởi tạo nrf24l01
  Serial.begin(9600);
  radio.begin();                     
  radio.setAutoAck(1);              
  radio.setDataRate(RF24_2MBPS);    // Tốc độ dữ liệu
  radio.setChannel(10);               // Đặt kênh
  radio.openReadingPipe(1,pipe);     
  radio.startListening(); 
  //Serial.println("init");

}

void loop(){

// nhận dữ liệu từ nrf
  if (radio.available()) {
      radio.read(&msg,sizeof(msg));
     }
     goc = 45+ (msg[18]*90)/255;
// Serial.println(goc);
Serial.println(msg[17]);
if(msg[17]<120) 
  {
   tien();
  }
 else if(msg[17]>=132)
  {
   lui();
  }
 else 
 {
  digitalWrite(4, LOW);
 }

// góc : 0.000 đến 180.000
  write(PB1,180-goc);// pin ~9 trên arduino  UNO R3
  msg[17] = 123;
//delay(5000);//5s
}

void tien()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  analogWrite(4,255-(((msg[17])*255)/123));
}

void lui()
{
  digitalWrite(3, HIGH);
  digitalWrite(2, LOW);
  analogWrite(4,255-(((msg[17])*255)/123));

}

