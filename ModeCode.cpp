#include <Arduino.h>
#include "Pumpkin.h"

int bumpAndLimit (int v, int b, int ll, int ul) {
  v += b;
  if (v < ll) {
    return ll;
  } else if (v > ul) {
    return ul;
  } else {
    return v;
  }
}

char * modeString(Mode _m) {
  switch (_m) {
    case MODE_NONE:
      return "NONE";
      break;
    case MODE_DIMGREY:
      return "DIMGREY";
      break;
    case MODE_RED:
      return "RED";
      break;
    case MODE_BLUE:
      return "BLUE";
      break;
    case MODE_GREEN:
      return "GREEN";
      break;
    case MODE_RAINBOW:
      return "RAINBOW";
      break;
    default:
      return "Unknown";
  } 
}

// generic superclass
void ModeCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
bool ModeCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
void ModeCode::requestModeEnd() { };
void ModeCode::finish(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
char * ModeCode::whoIAm() { return "superclass"; }

// mode that does nothing. use this to bootstrap at start
bool ModeNoneCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) {
  pumpkinColor->clear();
  return true; // switch modes immediately
}
char * ModeNoneCode::whoIAm() { return "None Code"; }

void ModeRedCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = 0;
  direction = 1;
  pC->clear();
}
bool ModeRedCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = bumpAndLimit (counter, direction, 0, 255);
  pC->setR(counter);
  if (counter <= 0 || counter >= 255) {
    direction = -direction;
  }
  return counter <= 0;
}
char * ModeRedCode::whoIAm() { return "Red Code"; }

void ModeBlueCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = 0;
  direction = 7;
  pC->clear();
}
bool ModeBlueCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = bumpAndLimit (counter, direction, 0, 255);
  pC->setB(counter);
  if (counter <= 0 || counter >= 255) {
    direction = -direction;
  }

  return counter <= 0;
}
char * ModeBlueCode::whoIAm() { return "Blue Code"; }


void ModeGreenCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = 0;
  direction = 2;
  pC->clear();
}
bool ModeGreenCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  counter = bumpAndLimit (counter, direction, 0, 255);
  pC->setG(counter);
  if (counter <= 0 || counter >= 255) {
    direction = -direction;
  }

  return counter <= 0;
}
char * ModeGreenCode::whoIAm() { return "Green Code"; }

void ModeDimGreyCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  pC->clear();
  startMillis = millis();
}
bool ModeDimGreyCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  unsigned long elapsedMillis = millis() - startMillis;
  pC->clear();
  
  if (elapsedMillis % 1000 > 500) {
   pC->setR(16);
   pC->setG(16);
   pC->setB(16);
   return false;
  }
  return true;
}
char * ModeDimGreyCode::whoIAm() { return "Dim Grey"; }
