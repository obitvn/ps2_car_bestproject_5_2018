#include <PS2X_lib.h>  //for v1.6
#include <SPI.h>
#include "RF24.h"

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        4  //14    
#define PS2_CMD        3  //15
#define PS2_SEL        2  //16
#define PS2_CLK        5  //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   false
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;
// config nrf
const uint64_t pipe = 0x73AC2D9FB5LL; // địa chỉ để phát
RF24 radio(9,10); //thay 10 thành 53 với mega
byte msg[3];
byte data_ps2[20];

void setup(){

  Serial.begin(57600);
  
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }

   // khoi tao nrf24l01
  radio.begin();                     
  radio.setAutoAck(1);               
  radio.setRetries(1,1);             
  radio.setDataRate(RF24_2MBPS);    // Tốc độ truyền
  radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
  radio.setChannel(10);               // Đặt kênh
  radio.openWritingPipe(pipe);        // mở kênh
  ps2x.read_gamepad(); //read controller
}

void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
   // ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed

    ps2x.read_gamepad(false, false);  //read controller but set rumble motor to offư

    data_ps2[16] = ps2x.Analog(PSS_LX); //123
    data_ps2[17] = ps2x.Analog(PSS_LY);//123
    data_ps2[18] = ps2x.Analog(PSS_RX); //123
    data_ps2[19] = ps2x.Analog(PSS_RY);// 132
    Serial.print(data_ps2[18], sizeof(data_ps2[18]));
    if (ps2x.NewButtonState()) {
    data_ps2[0] = ps2x.Button(PSB_START);  
    data_ps2[1] = ps2x.Button(PSB_SELECT);    
   // bốn phím lên xuống trái phải bên tay trái
    data_ps2[2] = ps2x.Button(PSB_PAD_UP);
    data_ps2[3] = ps2x.Button(PSB_PAD_RIGHT);
    data_ps2[4] = ps2x.Button(PSB_PAD_LEFT);
    data_ps2[5] = ps2x.Button(PSB_PAD_DOWN);
   //
    data_ps2[6] = ps2x.Button(PSB_L3);  // phim nhấn của joystick bên trái
    data_ps2[7] = ps2x.Button(PSB_R3); // phím nhấn của joystick bên phải
    data_ps2[8] = ps2x.Button(PSB_L2); // phím số 2 bên trái
    data_ps2[9] = ps2x.Button(PSB_R2); // phím số 2 bên phải
    
    data_ps2[10] = ps2x.Button(PSB_TRIANGLE);
    data_ps2[11] = ps2x.ButtonPressed(PSB_CIRCLE);        
    data_ps2[12] = ps2x.NewButtonState(PSB_CROSS);
    data_ps2[13] = ps2x.ButtonReleased(PSB_SQUARE);

  data_ps2[14] = ps2x.Button(PSB_L1);  // phím số 1 bên trái
  data_ps2[15] = ps2x.Button(PSB_R1);  // phím số 1 bên phải
    }

  radio.write(&data_ps2,sizeof(data_ps2));
}
