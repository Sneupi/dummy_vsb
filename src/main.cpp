
//-----------------------------------------------------------------------------
// VSB Dummy COM Port Device
// Sion Power 2023
//-----------------------------------------------------------------------------
// Description: Simulates "SN" cmd output, as well as echoes all serial
//-----------------------------------------------------------------------------
#include <Arduino.h>

//#define SIMPLE_OUTPUT // uncomment for simple, constant channel outputs
#define USE_TABS // uncomment to use tabs for whitespace

int cv[12];
int i = 1;
char timestampStr[9]; // Buffer to store the formatted timestamp

void setup() {
  // setup serial COM
  Serial.begin(115200);

  // Seed the random number generator
  randomSeed(analogRead(0));
  
  // Initialize cell voltage between 2000 and 2250 (0-4095 scale)
  for (int i = 0; i < 12; ++i) {
#ifdef SIMPLE_OUTPUT
    cv[i] = (i+1)*100;
#else
    cv[i] = random(2000, 2251);
#endif
  }
#ifndef SIMPLE_OUTPUT
  cv[3] = 1111; // make intentionally higher
  cv[6] = 3333; // make intentionally lower
#endif
Serial.println("VSB Dummy COM Device: Initialized");
}

void loop() {
  if (Serial.available()) {
    Serial.print("Received: ");
    while (Serial.available()) {
      Serial.print((char)Serial.read());
    }
  }
#ifndef SIMPLE_OUTPUT
  // change all cv by same percentage
  if (i == 1) { 
    float factor = (95 + random(11)) / 100.0; // random factor -5% to +5%
    for (int j = 0; j < 12; ++j) {
        cv[j] *= factor;
    }
  }
#endif

  // print the formatted string
  sprintf(timestampStr, "%08lu", millis() % 100000000);
  Serial.print(timestampStr);
  Serial.print(" DBG CV   ");
  Serial.print(i);
#ifdef USE_TABS
  Serial.print(":\t");
#else
  Serial.print((i <= 9) ? ":    " : ":   ");
#endif
  Serial.print(cv[i-1]);
  Serial.print("\r\n");
  delay(48);

  // print CH* for channel if needed
  if (i == 3) {  
    sprintf(timestampStr, "%08lu", millis() % 100000000);
    Serial.print(timestampStr);
#ifdef USE_TABS
    Serial.print(" DBG CH*  0:\t0\r\n");
#else
    Serial.print(" DBG CH*  0:    0\r\n");
#endif    
    delay(48);
  }
  else if (i == 6) { 
    sprintf(timestampStr, "%08lu", millis() % 100000000);
    Serial.print(timestampStr);
#ifdef USE_TABS
    Serial.print(" DBG CH*  1:\t0\r\n");
#else
    Serial.print(" DBG CH*  1:    0\r\n");
#endif
    delay(48);
  }
  else if (i == 9) {
    sprintf(timestampStr, "%08lu", millis() % 100000000);
    Serial.print(timestampStr);
#ifdef USE_TABS
    Serial.print(" DBG CH*  16:\t628\r\n");
#else
    Serial.print(" DBG CH*  16:   628\r\n"); 
#endif
    delay(48);
    }

  // increment x for the next iteration
  i = (i % 12) + 1;

  // delay for a short duration between prints (adjust as needed)
  delay(60);
}

