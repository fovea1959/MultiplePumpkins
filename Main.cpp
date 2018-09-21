#include <Arduino.h>

#include "Debug.h"
#include "Pumpkin.h"
#include "PumpkinLighters.h"

#define N_PUMPKINS 4

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
  
  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkinParms[i] = new PumpkinParms(i);
  }

  pumpkinParms[0]->add(new ModeInterval(MODE_DIMGREY, 1000));
  pumpkinParms[0]->add(new ModeInterval(MODE_RED, 1000));

  Serial.println("mode 0 test");
  pumpkinParms[0]->print();
  Serial.println("retest");
  for (int i = 0; i < pumpkinParms[0]->modeIntervalCount(); i++) {
    Serial << "retest #" << i << ": ";
    pumpkinParms[0]->getModeInterval(i)->print();
    Serial.println();
  }
  
  pumpkinParms[1]->add(new ModeInterval(MODE_RED, 1000));
  pumpkinParms[2]->add(new ModeInterval(MODE_BLUE, 1000));
  pumpkinParms[3]->add(new ModeInterval(MODE_GREEN, 1000));


  for (int i = 0; i < N_PUMPKINS; i++) {
    pumpkins[i] = new Pumpkin(i, pumpkinParms[i]);
  }

  pumpkinLighters[0] = new PumpkinLighter(&pixels, 0, -1, -1);
  pumpkinLighters[1] = new PumpkinLighter(&pixels, 1, -1, -1);
  pumpkinLighters[2] = new PumpkinLighter(&pixels, 2, -1, -1);
  pumpkinLighters[3] = new PumpkinLighter(&pixels, 3, -1, -1);
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
