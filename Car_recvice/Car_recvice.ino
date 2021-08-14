#include <TIMER1_SERVO.h>
#include "RF24.h"
const uint64_t pipe = 0x73AC2D9FB5LL; // địa chỉ phát
RF24 radio(8,10);//thay 10 thành 53 với mega
byte msg[20];
const int led = 3;
int led_st = 0, test=0;
unsigned char goc=90;
uint8_t cham =0, vua =0, nhanh =0;

void setup(){
port_attach(PB1);// chọn pin ~9
//timer2_setup();
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(4, OUTPUT);
digitalWrite(3, LOW);
delay(300);
msg[17] = 123;
msg[18] = 123;
delay(300);
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
  digitalWrite(3, LOW);
 }

// góc : 0.000 đến 180.000
if(msg[15] == 1) // // phím số 2 bên phải
{
  goc = 67.5+ (msg[18]*45)/255;
  write(PB1,180-goc);// pin ~9 trên arduino  UNO R3
  Serial.println(goc);
}
else if(msg[9] == 1)
{
  goc = 75+ (msg[18]*30)/255;
  write(PB1,180-goc);// pin ~9 trên arduino  UNO R3
}
else 
{
  goc = 45+ (msg[18]*90)/255;;
  write(PB1,180-goc);// pin ~9 trên arduino  UNO R3
}
}

void tien()
{
  
  digitalWrite(2, HIGH);
  digitalWrite(4, LOW);

  if(msg[8] == 1) // // phím số 2 bên phải
    {    
       analogWrite(3,(((255-msg[17])*255)/123)-273);
    }
 else if(msg[14] == 1)
    {
      analogWrite(3,(((255-msg[17])*170)/123)-182);
    }
 else 
    {
      analogWrite(3,(((255-msg[17])*100)/123)-108); 
    }
  
  
}

void lui()
{
  digitalWrite(2, LOW);
  digitalWrite(4, HIGH);
  if(msg[8] == 1) // // phím số 2 bên phải
    {    
       analogWrite(3,(((msg[17])*255)/123)-273);
    }
 else if(msg[14] == 1)
    {
      analogWrite(3,(((msg[17])*170)/123) -182);
    }
 else 
    {
      analogWrite(3,(((msg[17])*100)/123)-108); 
    }
 

}

// khởi tạo pwm timer 2
void timer2_setup()
{
TCCR2A =0; TCCR2B=0;
  //reset
    DDRD |= (1 << PD3);
    // PD3 output, (pin ~3)
    TCCR2A |= (1 << COM2B1);
    // so sánh thường ( none-inverting)
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    //  PWM Mode (răng cưa)
    TCCR2B |= (1 << CS22)|(1 << CS20);
    //  prescaler= 128, P_clock=16mhz/128=125 khz
    // F_pwm=125 khz/256=488.28 hz
    OCR2B = 0;
}

void pwm_out(uint8_t duty)
{
   OCR2B = duty; // (PD3) (pin 3)độ rộng xung =duty/256
}


