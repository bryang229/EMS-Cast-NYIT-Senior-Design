#include <LiquidCrystal_I2C.h>
#include <RotaryEncoder.h>
#include "RTClib.h"

// Note that this code attempts to "Program" all components, this code is NOT meant to be
// functional but to identify the needed components for a functional prototype
// This code should be broken down into functioning modules and rebuilt

// TODO: Add a way for interrupt to update the screen
// TODO: Add real time tracking

// Modules to break the code down into: (likely incomplete)
//  | 
//  |--Input------|
//  |             |--Rotary Encoder (Optional)
//  |             |--Buttons
//  |
//  |--Outputs----|
//  |             |--LCD (I2C) 
//  |             |--EMS Output (Using serial monitor for monitoring & control)
//  |--Others-----|
//  |             |--Real Time Clock (RTC)
//  |             |--Menu Control using Input (Rotary or Buttons)
//  |             |--Power handling 
//  |             |--EMS Reminder
//  |             |--E-Stop EMS 


// ---------------------------------------------------------------
// Arduino Nano Pin Mapping and Special Functions (Temporary)
// ---------------------------------------------------------------

// Digital Pins
// ---------------------------------------------------------------------------
// Pin  |  Interrupt |  PWM  |  Special Function        | Project Mapping
// ---------------------------------------------------------------------------
 // D0  |    No     |  No   |  RX (Serial)               | Unused
 // D1  |    No     |  No   |  TX (Serial)               | Unused
 // D2  |    Yes    |  No   |  External Interrupt 0      | ROT_A
 // D3  |    Yes    |  Yes  |  PWM/ External Interrupt 1 | ROT_B
 // D4  |    No     |  No   |                            | ROT_SW
 // D5  |    No     |  Yes  |  PWM                       | PW_SW
 // D6  |    No     |  Yes  |  PWM                       | SW_LED
 // D7  |    No     |  No   |                            | EN_CH1_POS
 // D8  |    No     |  No   |                            | EN_CH1_NEG
 // D9  |    No     |  Yes  |  PWM                       | EN_CH2_POS
 // D10 |    No     |  Yes  |  PWM / SPI (SS)            | EN_CH2_NEG
 // D11 |    No     |  Yes  |  PWM / SPI (MOSI)          | Unused
 // D12 |    No     |  No   |  SPI (MISO)                | Unused
 // D13 |    No     |  No   |  SPI (SCK) / Built-in LED  | Unused

// Analog Pins (Also usable as digital inputs/outputs A0 = D14, A1 = D15, etc.)
// ---------------------------------------------------------------------------
// Pin  |  Digital?  |  Special Function                | Project Mapping
// ---------------------------------------------------------------------------
 // A0  |   Yes    |  Analog Input                   | Unused
 // A1  |   Yes    |  Analog Input                   | Unused
 // A2  |   Yes    |  Analog Input                   | Unused
 // A3  |   Yes    |  Analog Input                   | Unused
 // A4  |   Yes    |  Analog Input / I2C (SDA)       | LCD / POT / RTC (DATA)
 // A5  |   Yes    |  Analog Input / I2C (SCL)       | LCD / POT / RTC (CLK)
 // A6  |   No     |  Analog Input Only              | Unused
 // A7  |   No     |  Analog Input Only              | Unused

// Notes:
// - External Interrupts: Available on pins D2 and D3.
// - PWM: Available on pins D3, D5, D6, D9, D10, and D11.
// - I2C: A4 (SDA) and A5 (SCL)
// - SPI: D10 (SS), D11 (MOSI), D12 (MISO), D13 (SCK)


// Input Devices parameters: 
  // Rotary Encoder (Pros: reduces button count| Cons: Not intutive and could be hard for people to use)
const int ROT_A = 2; // (CW Turn) On rising edges of ROT_A, ROT_A and ROT_B are the same 
const int ROT_B = 3; // (CCW Turn) on rising edges of ROT_A, ROT_A and ROT_B are opposite
const int ROT_SW = 4; 

RotaryEncoder *encoder = nullptr; //Creating a pointer for my encoder instance
int dir = 0;                 //Creating direction variable, 
// must use dir = (int)(encoder->getDirection()) but that is messy (See below)
//Creating a new function called getDir() which returns (int)(encoder->getDirection()) to reduce the messy
//-1 -> Counter Clock Wise || 0 -> Not moving || 1 -> Clock Wise

  //Power Button
    //TODO: Find safe and proper logic to turn off device, for now we are not using a 
    // Power Management IC (PMIC) so a on/off flag could work
const int PWR_SW = 5;  // Not too sure how to implement yet

//Output Devices parameters
  //LEDS (Probably will add two more for channel states)
const int PWR_LED = 6; // displays power state
bool pwer_state = true;

  //Enable pins (making them low turns off EMS channel ("in theory" - Oppenheimer)
const int EN_CH1_POS = 7; // These go to the solid state relays
const int EN_CH1_NEG = 8;
const int EN_CH2_POS = 9;
const int EN_CH2_NEG = 10;

  // LCD Parameters:  
const int addr = 0x20;
const int row = 2;
const int col = 16;

LiquidCrystal_I2C lcd(addr, col, row)

void setup() {
  //set up i/o 
  pinMode(ROT_SW, INPUT);
  pinMode(PWR_LED, OUTPUT);
  pinMode(EN_CH1_POS, OUTPUT);
  pinMode(EN_CH1_NEG, OUTPUT);
  pinMode(EN_CH2_POS, OUTPUT);
  pinMode(EN_CH2_NEG, OUTPUT);
  digitalWrite(PWR_LED, HIGH);

  //Handle rotary encoder
  //Setting up Rot Enc (LatchMode::FOUR3 is for when pinA and pinB are normally high)
  encoder = new RotaryEncoder(ROT_A, ROT_B, RotaryEncoder::LatchMode::FOUR3);
  //Attaching interrupts to pinA and pinB on falling edge
  attachInterrupt(digitalPinToInterrupt(ROT_A), checkPosition, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROT_B), checkPosition, FALLING);

  //Handle LCD
  lcd.init();
  lcd.backlight();
  display_message("Hello World");

}

void loop() {
  // put your main code here, to run repeatedly:

}

//Clears display and displays new message
void display_message(String msg){
    lcd.clear();
    lcd.print(msg,0, 0);
}

//Returns -1 for CCW, 0 for not moving and 1 for CW
int getDir(){
  return  (int)(encoder->getDirection());
}


//Interrupt Handlers (ISRs)

// Calls the RotaryEncoder's tick method to handle state management
void checkPosition() {
  encoder->tick(); // just call tick() to check the state.
}
