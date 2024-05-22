#include "Gateway.h"

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Setup OK");
  DESIoT_G_begin();
  // DESIoT_getUserMacros();
}

void loop() {
  // put your main code here, to run repeatedly:
  DESIoT_G_loop();
}