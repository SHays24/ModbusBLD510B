#include "BLD510B.h"

const int ModBusRx = 7;                              
const int ModBusTx = 4;
const int ModBusRTS = 5;
static constexpr uint8_t CTRL_EN = 1 << 0;
static constexpr uint8_t CTRL_FR = 1 << 1;
static constexpr uint8_t CTRL_BK = 1 << 2;
static constexpr uint8_t CTRL_NW = 1 << 3;
int speedSetPoint = 1000;

BLD510B motorA(1, true);

void setup() {
  pinMode(8, INPUT);
  Serial1.begin(9600, SERIAL_8N1, ModBusRx, ModBusTx);
  Serial1.setPins(-1, -1, -1, ModBusRTS);
  Serial1.setMode(UART_MODE_RS485_HALF_DUPLEX);
  delay(100);
}
void loop() {
  uint8_t control =
  CTRL_NW |
  CTRL_EN | 
  CTRL_FR;
  motorA.setControl(control, 2);
  delay(60);
  motorA.setSpeed(600);
}
