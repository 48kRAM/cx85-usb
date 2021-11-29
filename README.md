# cx85-usb

Arduino code to convert Atari CX85 keypad to a USB HID peripheral

## Atari CX85

The Atari CX85 keypad is a number pad w/ 4 additional function keys and - and +/<Enter> keys, which interfaced to the Atari 8-bit computers via the joystick port.

The CX85 can be modified internally by disconnecting the joystick cable, and connecting an Atmega 32u4-based Arduino (with 6 "analog" pins available)
to the 8 connections on the PC board. This can be done non-destructively, as the joystick cable disconnects using quick-connect terminals.

An Arduino board with 6 "analog" pins (A0-A5) is required because this code reads the entire portF in a single read.
Boards that have these pins include:
 * Adafruit ItsyBitsy 32u4 (tested, working)
 * Arduino Leonardo (technically works but is too large to install internally)
 * Arduino Micro
 * Pololu A-Star 32U4 Mini LV
 
