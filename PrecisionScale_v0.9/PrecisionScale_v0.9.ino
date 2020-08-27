/*
 *  PrecisionScale
 *
 *  Author: Dominic Hahn
 *  Date: 27 August 2020
 *  Author: Matthias Kesenheimer - @BartimaeusvUruk - https://github.com/MKesenheimer
 *  Date: 27 June 2020
 *
 *  Based on TM1637TinyDisplay library at https://github.com/jasonacox/TM1637TinyDisplay
 *  and HX711_ADC library at https://github.com/olkal/HX711_ADC
 *
 * Libraries to install:
 * - TM1637TinyDisplay
 * - HX711_ADC Arduino Library
 * 
 * Programming with JTAGICE3: 
 * - test: $ avrdude -cjtag3isp -pm328p -Pusb -B22
 * - vim /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/programmers.txt
 *   -> add/modify line "jtag3isp.program.extra_params=-Pusb -B22"
 * - choose Programmer "Atmel JTAGICE3 (ISP Mode)"
 * - upload via Programmer (cmd+shift+u)
 */

#include <HX711_ADC.h>
#include <TM1637TinyDisplay.h>

// wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int DISPLAY_DOUT_PIN = 4;
const int DISPLAY_SCK_PIN = 5;
const int BUTTON_PIN = 6;

TM1637TinyDisplay display(DISPLAY_SCK_PIN, DISPLAY_DOUT_PIN);
HX711_ADC loadCell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

void setup() {
  pinMode(BUTTON_PIN, INPUT);
    
  display.setBrightness(BRIGHT_3);
  display.showString("----");

  // set here your calibration value (obtained by sketch .ino)
  float calibrationValue = 12103.11;

  // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  long stabilizingtime = 4000;
  loadCell.begin();
  loadCell.start(stabilizingtime, true);
  if (loadCell.getTareTimeoutFlag()) {
    display.showString("Err");
    while (1);
  } else {
    loadCell.setCalFactor(calibrationValue);
    display.clear();
  }
  
  loadCell.tare();
}

bool checkTare() {
  // Variablen für entprellten Taster
  bool buttonOn = false;
  bool buttonStatus = false;
  bool buttonOut = false;

  // read tare 
  buttonOn = digitalRead(BUTTON_PIN);
  if (buttonOn == true) {
    delay(5);
    if (buttonOn == true && buttonStatus == false) {
      buttonOut = true;
      buttonStatus=true;
    }
    else if (buttonOn == true && buttonStatus == true) {
      buttonStatus = true;
      buttonOut = false;
    }
  } else if (buttonOn == false && buttonStatus == true) {
    buttonStatus = false;
    buttonOut = false;
  }

  return buttonOut;
}

void displayWeight(float value) {
  if (value < 1 && value >= 0) {
    if (value >= 0 && value < 0.005) {
      display.showString(" ", 1, 0);
      display.showNumberDec(0, (0x80 >> 0), true, 3, 1);
    } else {
      value = value + 0.005;
      display.showString(" ", 1, 0);
      display.showNumberDec(value * 100, (0x80 >> 0), true, 3, 1);
    }   
  }
  
  if (value < 0 && value > -1) {
    if (value > -0.005 && value < 0) {
      display.showString(" ", 1, 0);
      display.showNumberDec(0, (0x80 >> 0), true, 3, 1);
    } else {
      value = value - 0.005;
      display.showString("-", 1, 0);
      display.showNumberDec(value * (-100), (0x80 >> 0), true, 3, 1);
    }   
  }
     
  if (value >= 1)  {
    value = value + 0.005;
    display.showNumberDec(value * 100, (0x80 >> 1), false, 4);
  }
  
  if (value <= -1) {
    value = value - 0.005;
    display.showNumberDec(value * 100, (0x80 >> 1), false, 4);
  }
}

void loop() {
  if (checkTare()) {
    display.showString("tara");
    loadCell.tare();  
  }

  float value;
  // check for new data/start next conversion:
  // get smoothed value from the dataset:
  if (loadCell.update()) {
    value = loadCell.getData();
  }
    
  // set small values artificially to zero
  if (abs(value) < 0.005) value = 0;
   
  displayWeight(value);
}
