#include <Arduino.h>

#include "Debug.h"
#include "Pumpkin.h"
#include "PumpkinLighters.h"

#define N_PUMPKINS 4

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
    pumpkins[i] = new Pumpkin(i, new PumpkinParms());
  }

  pumpkins[0]->setMode(MODE_DIMGREY);
  pumpkins[1]->setMode(MODE_RED);
  pumpkins[2]->setMode(MODE_BLUE);
  pumpkins[3]->setMode(MODE_GREEN);
//  pumpkins[4]->setMode(MODE_DIMGREY);

  pumpkinLighters[0] = new PumpkinLighter(&pixels, 1, -1, -1);
  pumpkinLighters[1] = new PumpkinLighter(&pixels, 2, -1, -1);
  pumpkinLighters[2] = new PumpkinLighter(&pixels, 3, -1, -1);
  pumpkinLighters[3] = new PumpkinLighter(&pixels, 4, -1, -1);
//  pumpkinLighters[4] = new PumpkinLighter(&pixels, 4, -1, -1);

  for (int i = 0; i < N_PUMPKINS; i++) {

#ifdef XXXXXX
    Serial << "pumpkin " << i << " = " << pumpkins[i]->getId() << "\r\n";

    Serial.print ("pumpkin ");
    Serial.print (i);
    Serial.print (": color @ ");
    Serial.print ((int) pumpkins[i]->getPumpkinColor());
    Serial.print (" = ");
    Serial.print (pumpkins[i]->getPumpkinColor()->getId());
    Serial.print (", mode = ");
    Serial.print (pumpkins[i]->getCurrentMode());
    Serial.println();
#endif    
  }

  pixels.begin();
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkins[i] -> update();
  }
  for (int i = 0; i < N_PUMPKINS; i++) {
    PumpkinColor * pC = pumpkins[i]->getPumpkinColor();

#ifdef xxx
    Serial << "pumpkin "<< i<< " color = ";
    pC->print();
    Serial << "\r\n";
#endif
    
    pumpkinLighters[i] -> update (pC);
  }

  pixels.show();
}
