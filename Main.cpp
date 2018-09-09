#include <Arduino.h>
#include "Pumpkin.h"

#define N_PUMPKINS 2

Pumpkin *pumpkins[N_PUMPKINS];
PumpkinColor *pumpkinColors[N_PUMPKINS];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  for (int i = 0; i < N_PUMPKINS; i++) {
    PumpkinColor * pumpkinColor = new PumpkinColor(i);
    pumpkinColors[i] = pumpkinColor;
    Pumpkin * pumpkin = new Pumpkin(i, pumpkinColor);
    pumpkins[i] = pumpkin;
  }

  for (int i = 0; i < N_PUMPKINS; i++) {
    Serial.print ("pumpkin ");
    Serial.print (i);
    Serial.print (" = ");
    Serial.println (pumpkins[i]->getId());

    Serial.print ("pumpkin color ");
    Serial.print (i);
    Serial.print (" @ ");
    Serial.print ((int) pumpkinColors[i]);
    Serial.print (" = ");
    Serial.println (pumpkinColors[i]->getId());
    
    Serial.print ("pumpkin color for pumpkin ");
    Serial.print (i);
    Serial.print (" @ ");
    Serial.print ((int) pumpkins[i]->getPumpkinColor());
    Serial.print (" = ");
    Serial.println (pumpkins[i]->getPumpkinColor()->getId());
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
