#include "Pumpkin.h"

    Pumpkin::Pumpkin(int i, PumpkinParms * _pumpkinParms) {
      id = i;
      pP = _pumpkinParms;
      pC = new PumpkinColor(i);
      // Serial << "pumpkin " << i << " got pumpkinColor " << pC->getId() << "\r\n";

      currentMode = MODE_NONE;
      currentModeCode = &modeNoneCode;

      newMode = MODE_NONE;
      modeWasChanged = 1;
      modeChangeMillis = 0;

      ModeCode * mc1 = &modeNoneCode;
      ModeCode * mc2 = &modeRedCode;
      Serial << "generic: " << mc1->whoIAm() << ", " << mc2->whoIAm() << "\r\n";
      Serial << "typed: " << modeNoneCode.whoIAm() << ", " << modeRedCode.whoIAm() << "\r\n";
    }

    int Pumpkin::getId() {
      return id;
    }

    Mode Pumpkin::getCurrentMode() {
      return currentMode;
    }

    PumpkinColor * Pumpkin::getPumpkinColor() {
      return pC;
    }

    void Pumpkin::setMode (Mode m) {
      newMode = m;
      modeWasChanged = 1;
      Serial << "setMode called for " << m << "\r\n";
    }

#undef DEBUG_PUMPKIN
    void Pumpkin::update()
    {
      unsigned long currentMillis = millis();

      if (modeWasChanged) {
#ifdef DEBUG_PUMPKIN
        Serial << "mode was changed. setting up for mode " << newMode << " " << modeString(newMode) << "\r\n";
#endif
        currentModeCode->finish(pP, pC);
        switch (newMode) {
          case MODE_NONE:
            currentModeCode = &modeNoneCode;
            break;
          case MODE_RED:
            currentModeCode = &modeRedCode;
            break;
          case MODE_BLUE:
            currentModeCode = &modeBlueCode;
            break;
          case MODE_GREEN:
            currentModeCode = &modeGreenCode;
            break;
          case MODE_DIMGREY:
            currentModeCode = &modeDimGreyCode;
            break;
          default:
            Serial << "unhandled newMode: " <<  currentMode << "\r\n";
            currentModeCode = &modeNoneCode;
        } 
        currentMode = newMode;
        currentModeCode->init(pP, pC);
        modeWasChanged = 0;
      }

#ifdef DEBUG_PUMPKIN
        Serial << "running: " << currentModeCode->whoIAm() << "\r\n";
#endif
      
      bool okToChange = currentModeCode->update(pP, pC);

#ifdef DEBUG_PUMPKIN
        Serial << "colors: ";
        pC->print();
        Serial.println();
#endif
      
      if (okToChange && (currentMillis > modeChangeMillis)) {
#ifdef DEBUG_PUMPKIN
        Serial << "ok to change: " << okToChange << ", current millis = " << currentMillis << ", mode change millis: " << modeChangeMillis << "\r\n";
#endif
        ModeInterval * mi = pP->getRandomModeInterval();

#ifdef DEBUG_PUMPKIN
        Serial << "got modeInterval ";
        mi->print();
        Serial.println();
#endif

        modeWasChanged = 1;
        newMode = mi-> getMode();
        int tlength = mi -> getTLength();
        int variation = mi -> getVariation();
        int v = random(variation) - (variation / 2);
        if (v < 0) v = 0;
        modeChangeMillis = currentMillis + tlength + v;

#ifdef DEBUG_PUMPKIN
        Serial << "mode " << newMode << modeString(newMode) << " was picked, next change at " << modeChangeMillis << "\r\n";
#endif
      }

    }


/* -------------------------- PumpkinParms methods ----------------------------- */

PumpkinParms::PumpkinParms(int _id) {
  id = _id;
  firstMi = NULL;
  lastMi = NULL;
  nMi = 0;
}
void PumpkinParms::add (ModeInterval * _mi) {
  if (lastMi == NULL) {
    firstMi = _mi;
    lastMi = _mi;
  } else {
    lastMi->setNext(_mi);
    lastMi = _mi;
  }
  nMi++;
}
void PumpkinParms::print() {
  Serial << "Pumpkin parms #" << id << "\r\n";
  ModeInterval * t = firstMi;
  while (t != NULL) {
    t->print();
    t = t->getNext();
    Serial.println();
  }
}
ModeInterval * PumpkinParms::getModeInterval(int _n) {
  ModeInterval * t = firstMi;
  int i = 0;
  while (t != NULL && i < _n) {
    t = t->getNext();
    i++;
  }
  return t;
}
ModeInterval * PumpkinParms::getRandomModeInterval() {
  int i = random(nMi);
  return getModeInterval(i);
}
int PumpkinParms::modeIntervalCount() {
  return nMi;
}

/* -------------------------- ModeInterval methods ----------------------------- */
ModeInterval::ModeInterval (int _mode, int _tlength) {
  next = NULL;
  mode = _mode;
  tlength = _tlength;
  variation = 0;
}
ModeInterval::ModeInterval (int _mode, int _tlength, int _variation) {
  next = NULL;
  mode = _mode;
  tlength = _tlength;
  variation = _variation;
}
void ModeInterval::setNext( ModeInterval * _next) {
  next = _next;
}
ModeInterval * ModeInterval::getNext() { return next; }
int ModeInterval::getMode() { return mode; }
int ModeInterval::getTLength() { return tlength; }
int ModeInterval::getVariation() { return variation; }
void ModeInterval::print() {
  Serial << "mode interval: " << mode << ", tlength: " << tlength << ", variation: " << variation;
}

/* -------------------------- PumpkinColor methods ----------------------------- */

PumpkinColor::PumpkinColor(uint8_t _id) {
  id = _id + 10;
  clear();
}
int PumpkinColor::getId() { return id; }
void PumpkinColor::setR (int v) { r = v; }
void PumpkinColor::setG (int v) { g = v; }
void PumpkinColor::setB (int v) { b = v; }
void PumpkinColor::setY (int v) { y = v; }
void PumpkinColor::setUv (int v) { uv = v; }
int PumpkinColor::getR() { return r; }
int PumpkinColor::getG() { return g; }
int PumpkinColor::getB() { return b; }
int PumpkinColor::getY() { return y; }
int PumpkinColor::getUv() { return uv; }
void PumpkinColor::print() {
  Serial << "r: " << r << ", g: " << g << ", b: " << b;
}
void PumpkinColor::clear() {
  r = 0;
  g = 0;
  b = 0;
  y = 0;
  uv = 0;
}

