# PrecisionScale
A precision scale based on the Arduino nano.
Without much effort, a precision of about one-tenth of a gram can be achieved.

## Prerequisites
- 1 Arduino Nano (328p)
- 1 4-Digit LED Display
- 1 100g load cell
- HX711 board

## Wiring
The wiring is very simple and could be done with Dupont cables only.
![PrecisionScaleWiring_bb](https://github.com/MKesenheimer/PrecisionScale/blob/master/PrecisionScaleWiring_bb.png)

## Usage
Before using the scale, you have to determine the calibration factor.
This could be done by uploading the sketch `Calibration.ino` from the `HX711_ADC` library.
After the calibration routine has finished, a calibration value is obtained which then can be inserted into the sketch `PrecisionScale_v0.9.ino`.

Apart from that, using the scale should be self-explanatory.

## Construction
STL-files for 3D-printing a case for the scale are included.
![PrecisionScaleConstruction](https://github.com/MKesenheimer/PrecisionScale/blob/master/Construction1.png)