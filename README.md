Smart Medication Reminder Box

Overview

The Smart Medication Reminder Box is an Arduino-based assistive device designed to remind users to take their medication at predefined times. The system combines visual, audible and interactive feedback to support people who may forget their medication or require additional reminders.

Hardware

 Arduino Uno R3
16×2 I2C LCD Display
 RTC DS1302 Real-Time Clock Module
LED
Buzzer
Push Button (or Joystick Button)
Breadboard
Jumper Wires
USB Cable

Required Libraries

Install the following Arduino libraries before uploading the code:

Wire
LiquidCrystal_I2C
ThreeWire
Rtc by Makuna (Version 2.5.0)

Hardware Setup

1. Connect the LCD display to the Arduino using the I2C interface.
2. Connect the RTC DS1302 module to the digital pins.
3. Connect the LED to the output pin.
4. Connect the buzzer to the buzzer output pin.
5. Connect the push button (or joystick button) to the input pin.
6. Power the Arduino via USB.

### LCD Display

| Arduino | LCD Display |
|---------|-------------|
| GND | GND |
| 5V | VCC |
| A4 | SDA |
| A5 | SCL |


### RTC DS1302 Module

| RTC | Breadboard |
|-----|------------|
| VCC | J25 |
| GND | J24 |
| CLK | J23 |
| DAT | J22 |
| RST | J21 |


### Breadboard Connections

| Pinboard | Breadboard | Arduino |
|----------|------------|---------|
| I25 | + Line | |
| I24 | - Line | |
| I23 | | Digital 6 |
| I22 | | Digital 7 |
| I21 | | Digital 8 |


### LEDs

| LED long leg on breadboard | LED short leg on breadboard | Arduino |
|----------------------------|-----------------------------|----------|
| C10 | C12 | 13 |
| C20 | C22 | 12 |
| C30 | C32 | 11 |


### Buzzer

| Buzzer left leg on breadboard | Buzzer right leg on breadboard | Arduino |
|-------------------------------|--------------------------------|----------|
| A1 | A3 | A3 |
| | A3 and a cable to - Line | |



### Joystick

| Joystick | Arduino | Breadboard |
|----------|----------|------------|
| SW | 2 | |
| +5V | | + Line |
| GND | | - Line |



How the System Works

After powering on the device:

1. The LCD displays “System bereit” (“System ready”).
2. The RTC module continuously provides the current time.
3. The LCD permanently displays the current time.
4. Three medication reminder times are stored in the program.
5. When one of these times is reached:
the LCD displays “MEDIKAMENT!”,
the LED starts blinking,
the buzzer sounds repeatedly.
6. The user confirms the reminder by pressing the button.
7. The alarm stops, the LED turns off, and the display shows “Alarm gestoppt” before returning to the clock.

Current Reminder Times

The current prototype is configured for three daily reminders:

08:00
12:00
20:16

These values can easily be changed in the Arduino source code.


Features

Real-time clock using RTC module
Automatic medication reminders
LCD notification
Audible alarm (Buzzer)
Visual alarm (LED)
User confirmation via button
Continuous time display


Future Improvements

Possible future extensions include:

Adjustable reminder times through buttons or a menu
Multiple medication schedules
Battery backup
Bluetooth or Wi-Fi notifications
Mobile app integration
Larger display for improved accessibility


Sources that helped us with the implementation:

https://circuitdigest.com/microcontroller-projects/arduino-medicine-reminder 

https://justdoelectronics.com/medicine-reminder-with-arduino/ 

https://www.youtube.com/watch?v=UA0kP0PIl4A

https://youtu.be/ZSdc1dorKFM?si=2guMzU51YCGyOplA



