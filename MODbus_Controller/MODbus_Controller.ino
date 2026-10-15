#include "BLD510B.h"

const int ModBusRx = 7;                              
const int ModBusTx = 4;
const int ModBusRTS = 5;
static constexpr uint8_t CTRL_EN = 1 << 0;
static constexpr uint8_t CTRL_FR = 1 << 1;
static constexpr uint8_t CTRL_BK = 1 << 2;
static constexpr uint8_t CTRL_NW = 1 << 3;
int speedSetPoint = 1000;

BLD510B motorA(2, true); // set motor Id to 1 and motor direction
BLD510B motorB(100, true);

void setup() {
  pinMode(8, INPUT);
  Serial1.begin(9600, SERIAL_8N1, ModBusRx, ModBusTx);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW); // Default to receive mode

  Serial1.begin(9600, SERIAL_8N1, ModBusRx, ModBusTx);
  
  // Note: Do NOT call Serial1.setMode(UART_MODE_RS485_HALF_DUPLEX)
  // or Serial1.setPins(-1, -1, -1, ModBusRTS) when doing manual toggling.
  delay(100);
  uint8_t control =
    CTRL_NW |
    CTRL_EN | 
    CTRL_FR;
  motorA.setSpeed(0);
  delay(150);
  motorA.setControl(control, 2);
  delay(150);
  motorB.setSpeed(0);
  delay(150);
  motorB.setControl(control, 2);
}
void loop() {
  //delay(100);
  motorA.setSpeed(600);
  delay(300);
  motorA.setSpeed(0);
  //delay(300);
  //motorB.setSpeed(600);
  //delay(300);
  //motorB.setSpeed(0);
  delay(300);
}
