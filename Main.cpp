#include <Arduino.h>
#include "Pumpkin.h"
#include "PumpkinLighters.h"

#define N_PUMPKINS 2

Pumpkin * pumpkins[N_PUMPKINS];
PumpkinLighter * pumpkinLighters[N_PUMPKINS];

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  for (int i = 0; i < N_PUMPKINS; i++) {
    Pumpkin * pumpkin = new Pumpkin(i, new PumpkinParms());
    pumpkins[i] = pumpkin;
  }

  pumpkins[0]->setMode(MODE_RED);
  pumpkins[1]->setMode(MODE_BLUE);

  pumpkinLighters[0] = new PumpkinLighter(&pixels, 1, 2, 3);
  pumpkinLighters[1] = new PumpkinLighter(&pixels, 5, 6, 7);

  for (int i = 0; i < N_PUMPKINS; i++) {
    Serial.print ("pumpkin ");
    Serial.print (i);
    Serial.print (" = ");
    Serial.println (pumpkins[i]->getId());

    Serial.print ("pumpkin ");
    Serial.print (i);
    Serial.print (": color @ ");
    Serial.print ((int) pumpkins[i]->getPumpkinColor());
    Serial.print (" = ");
    Serial.print (pumpkins[i]->getPumpkinColor()->getId());
    Serial.print (", mode = ");
    Serial.print (pumpkins[i]->getCurrentMode());
    Serial.println();
  }

  pixels.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkins[i] -> update();
  }
  for (int i = 0; i < N_PUMPKINS; i++) {
    PumpkinColor * pC = pumpkins[i]->getPumpkinColor();
    Serial.print ("pumpkin ");
    Serial.print (i);
    Serial.print (" color = ");
    pC->print();
    Serial.println();
    pumpkinLighters[i] -> update (pC);
  }
  //pixels.setPixelColor (0, 128, 128, 128);
  pixels.show();
}
