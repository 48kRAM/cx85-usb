#include <Keyboard.h>

// Uncomment this line to send debug output on CDC serial
//#define DEBUG

// Uncomment this line to avoid sending actual keystrokes
// during testing / development
//#define NOKEYS

// How many ms to hold Enter to change modes
#define MODE_DELAY 5000

/* Key layout map to hex values
    30    51  52  53   73
    50    41  42  43
    40    61  62  63   72
    60      70    71
*/

uint8_t pf;
uint8_t isKeyDown = 0x0;
uint8_t obsmode = 0x0;
unsigned long keyDownMillis=0x0;

/* This code expects to see an Arduino board based on
 * the Atmega 32u4, with the "analog" pins on port F.
 * Supported boards include:
 *  Arduino Leonardo
 *  Adafruit Itsy Bitsy 32u4
 *  Arduino Pro Micro (untested)
 */

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  #ifdef DEBUG
  Serial.begin(9600);
  Serial.setTimeout(50);
  delay(2000);
  #endif
  Keyboard.begin();
  pf = PINF;
  if ((pf & 0B10000000) == 0x0) {
    //Set mode to OBS
    obsmode = 0x1;
    #ifdef DEBUG
    Serial.println(F("Enabling OBS mode"));
    #endif
    while ((PINF & 0B10000000) == 0x0) { delay(50); }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  pf = PINF;
  // Bit 7 goes low whenever a key is pressed
  if ((pf & 0B10000000) == 0x0) {
    if (isKeyDown == 0x0) {
      keyDownMillis=millis();
      pf &= 0B01110011;
      #ifdef DEBUG
      Serial.print("0x");
      Serial.println(pf, HEX);
      #endif
      isKeyDown = 0x1;
      if (obsmode) {
        if (pf == 0x70) { send_A_char('a'); }
        if (pf == 0x71) { send_A_char('v'); }
        if (pf == 0x72) { send_char(KEY_RETURN); }
        if (pf == 0x73) { send_A_char(KEY_F6); }

        if (pf == 0x61) { send_A_char('1'); }
        if (pf == 0x62) { send_A_char('2'); }
        if (pf == 0x63) { send_A_char('3'); }

        if (pf == 0x41) { send_A_char('4'); }
        if (pf == 0x42) { send_A_char('5'); }
        if (pf == 0x43) { send_A_char('6'); }
        if (pf == 0x51) { send_A_char('7'); }
        if (pf == 0x52) { send_A_char('8'); }
        if (pf == 0x53) { send_A_char('9'); }

        if (pf == 0x30) { // Escape
          #ifdef DEBUG
          Serial.println("Esc");
          #endif
          send_A_char(KEY_F5);
        }

        if (pf == 0x50) { // No
          #ifdef DEBUG
          Serial.println("No");
          #endif
          send_A_char(KEY_F6);
        }

        if (pf == 0x40) { // Delete
          #ifdef DEBUG
          Serial.println("Del");
          #endif
          send_A_char(KEY_F7);
        }
        if (pf == 0x60) { // Yes
          #ifdef DEBUG
          Serial.println("Yes");
          #endif
          send_A_char(KEY_F8);
        }
      
      } else {
        if (pf == 0x70) { send_char('0'); }
        if (pf == 0x71) { send_char('.'); }
        if (pf == 0x72) { send_char('+'); }
        if (pf == 0x73) { send_char('-'); }

        if (pf == 0x61) { send_char('1'); }
        if (pf == 0x62) { send_char('2'); }
        if (pf == 0x63) { send_char('3'); }

        if (pf == 0x41) { send_char('4'); }
        if (pf == 0x42) { send_char('5'); }
        if (pf == 0x43) { send_char('6'); }

        if (pf == 0x51) { send_char('7'); }
        if (pf == 0x52) { send_char('8'); }
        if (pf == 0x53) { send_char('9'); }

        if (pf == 0x30) { // Escape
          #ifdef DEBUG
          Serial.println("Esc");
          #endif
          send_char(KEY_ESC);
        }

        if (pf == 0x40) { // Delete
          #ifdef DEBUG
          Serial.println("Del");
          #endif
          send_A_char(KEY_BACKSPACE);
        }
      }
    } else {
      // iskeydown not zero
      if (pf == 0x72 && (millis() - keyDownMillis > MODE_DELAY) ) {
        #ifdef DEBUG
        Serial.print(F("Changing key mode to "));
        if (obsmode) { Serial.println(F("keypad mode")); }
        else { Serial.println(F("OBS mode")); }
        #endif
        if (obsmode ) { obsmode = 0x0; }
        else { obsmode = 0x1; }
        keyDownMillis=millis();
      }
    }
//End iskeydown == 0x0
  } else {
    if (isKeyDown)     {
      #ifdef DEBUG
      Serial.println(F("Keys up"));
      #endif
      Keyboard.releaseAll();
      isKeyDown = 0x0;
    }
  }
  delay(10);
}


void send_char(char c) {
  #ifdef DEBUG
  if (c < 128 && c > 32) {
    Serial.println(c);
  }
  #endif
  #ifndef NOKEYS
  Keyboard.press(c);
  #endif
}

void send_CA_char(char c) {
  #ifdef DEBUG
  if (c < 128 && c > 32) {
    Serial.print("Ctrl-Alt-");
    Serial.println(c);
  }
  #endif
  #ifndef NOKEYS
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(c);
  #endif
}

void send_A_char(char c) {
  #ifdef DEBUG
  if (c < 128 && c > 32) {
    Serial.print("Alt-");
    Serial.println(c);
  }
  #endif
  #ifndef NOKEYS
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(c);
  #endif
}
