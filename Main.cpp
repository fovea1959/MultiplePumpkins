#include <Arduino.h>

#include "Pumpkin.h"
#include "PumpkinLighters.h"


#define N_PUMPKINS 8

#define YES

Pumpkin * pumpkins[N_PUMPKINS];
PumpkinParms * pumpkinParms[N_PUMPKINS];
PumpkinLighter * pumpkinLighters[N_PUMPKINS];
unsigned long lastMillis;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  randomSeed(analogRead(0));
  
  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkinParms[i] = new PumpkinParms(i);
  }

  pumpkinParms[0]->add(new ModeInterval(MODE_RAINBOW, ULONG_MAX));

#ifdef DEBUG1
  Serial.println("mode 0 test");
  pumpkinParms[0]->print();
  Serial.println("retest");
  for (int i = 0; i < pumpkinParms[0]->modeIntervalCount(); i++) {
    Serial << "retest #" << i << ": ";
    pumpkinParms[0]->getModeInterval(i)->print();
    Serial.println();
  }
#endif
  
  pumpkinParms[1]->add(new ModeInterval(MODE_RED, ULONG_MAX));
  pumpkinParms[2]->add(new ModeInterval(MODE_BLUE, ULONG_MAX));
  pumpkinParms[3]->add(new ModeInterval(MODE_GREEN, ULONG_MAX));
  
  pumpkinParms[4]->add(new ModeInterval(MODE_DIMGREY, 5000));
  pumpkinParms[4]->add(new ModeInterval(MODE_RED, 5000, 5000));

  pumpkinParms[7]->add(new ModeInterval(MODE_RAINBOW, ULONG_MAX));

  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkins[i] = new Pumpkin(i, pumpkinParms[i]);
    pumpkinLighters[i] = new PumpkinLighter(&pixels, i, -1, -1);
  }


#ifdef DEBUG1
  for (int i = 0; i < N_PUMPKINS; i++) {
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
  }
#endif    

  pixels.begin();
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, 0, 0, 0);
  }
  pixels.show();

  lastMillis = millis();  
}

void loop() {
  unsigned long now = millis();
  if (now - lastMillis > 10) {
    int n_pumpkins = N_PUMPKINS;
    for (int i = 0; i < n_pumpkins; i++) {
      pumpkins[i] -> update();
    }
    for (int i = 0; i < n_pumpkins; i++) {
      PumpkinColor * pC = pumpkins[i]->getPumpkinColor();
      pumpkinLighters[i] -> update (pC);
    }
  
    pixels.show();
    lastMillis = now;
  } else {
    delay(1);
  }
}
