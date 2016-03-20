#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
// ensure this library description is only included once

#include <LiquidCrystal.h>
#include <AFMotor.h>

int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons()
{
  int adc_key_in = analogRead(0);      // read the value from the sensor
  Serial.println(adc_key_in);
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnSELECT;  // when all others fail, return this...
}

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
AF_Stepper motor(448, 1);

float pos = 0;
float cur_pos = 0;
float speed = 5;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0, 0);
  motor.setSpeed(20);
}

void loop()
{
  delay(50);
  lcd.setCursor(0, 0);           // move cursor to second line "1" and 9 spaces over
  lcd.print("p=");
  lcd.print(pos);      // display seconds elapsed since power-up
  lcd.print("    ");

  lcd.setCursor(0, 1);           // move to the begining of the second line
  lcd.print("s=");
  lcd.print(speed);
  lcd.print("     ");

  int lcd_key = read_LCD_buttons();  // read the buttons

  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnLEFT:
      {
        speed -= 0.1;
        if (speed < 0.1) speed = 0.1;
        break;
      }
    case btnRIGHT:
      {
        speed += 0.1;
        if (speed > 50) speed = 50;
        break;
      }
    case btnUP:
      {
        pos -= speed;
        break;
      }
    case btnDOWN:
      {
        pos += speed;
        break;
      }
     case btnNONE:
      {
      motor.release();
      break;
      }
  }
  int dp = pos - cur_pos;

  if (dp > 0) {
    motor.step(dp, BACKWARD, DOUBLE);
    cur_pos += dp;
  }
  if (dp < 0) {
    motor.step(-dp, FORWARD, DOUBLE);
    cur_pos += dp;
  }
}
