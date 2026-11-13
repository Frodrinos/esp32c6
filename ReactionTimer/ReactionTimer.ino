#include <Arduino.h>

unsigned long greenMs;
bool resultStatus = false;

void setup() {
  // The pull-up keeps GPIO9 HIGH until the BOOT button pulls it LOW
  pinMode(9, INPUT_PULLUP);

#ifdef RGB_BUILTIN
  // Keep the LED off while the player waits for the signal
  rgbLedWrite(RGB_BUILTIN, 0, 0, 0);

  // Set the Serial Monitor to 115200 baud to read the results
  Serial.begin(115200);

  // Record when the wait begins and pick a delay from 1000 to 3999 ms
  unsigned long waitStarted = millis();
  int buttonDelay = random(1000, 4000);

  // End the round if the player presses BOOT before the green signal
  while (millis() - waitStarted < buttonDelay) {
    if (digitalRead(9) == LOW) {
      Serial.println("BRUH");
      resultStatus = true;
      return;
    }
  }

  // Turn the LED green and record when the player can start responding
  rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);
  greenMs = millis();
#endif
}

void loop() {
  // Measure the time from the green signal to the first BOOT press
  if (digitalRead(9) == LOW && !resultStatus) {
    unsigned long reactionMs = millis() - greenMs;
    rgbLedWrite(RGB_BUILTIN, 0, 0, 0);
    Serial.println(reactionMs);

    // A held button should produce only one result per round
    resultStatus = true;
  }
}
