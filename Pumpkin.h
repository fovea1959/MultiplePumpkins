#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

#define DEBUG_ON

#include "Debug.h"

#include "PumpkinParms.h"
#include "ModeInterval.h"

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
ModeInterval * ModeInterval::getNext() {
  return next;
}
int ModeInterval::getMode() {
  return mode;
}
int ModeInterval::getTLength() {
  return tlength;
}
int ModeInterval::getVariation() {
  return variation;
}
void ModeInterval::print() {
  Serial << "mode interval: " << mode << ", tlength: " << tlength << ", variation: " << variation;
}




class PumpkinColor
{
  public:
    int id, r, g, b, y, uv;
    PumpkinColor(uint8_t _id) {
      id = _id + 10;
      clear();
    }
    int getId() {
      return id;
    }
    void setR (int v) {
      r = v;
    }
    void setG (int v) {
      g = v;
    }
    void setB (int v) {
      b = v;
    }
    void setY (int v) {
      y = v;
    }
    void setUv (int v) {
      uv = v;
    }
    void updateLeds() {
      
    }
    void print() {
      Serial << "r: " << r << ", g: " << g << ", b: " << b;
    }
    void clear() {
      r = 0;
      g = 0;
      b = 0;
      y = 0;
      uv = 0;
    }

};

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


typedef int Mode;
#define MODE_NONE 0
#define MODE_DIMGREY 1
#define MODE_RED 2
#define MODE_BLUE 3
#define MODE_GREEN 4

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
    default:
      return "Unknown";
  } 
}

// generic superclass
class ModeCode
{
  public:
    virtual void init(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
    virtual bool update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
    virtual void finish(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) { };
    virtual char * whoIAm() { return "superclass"; }
};

class ModeNoneCode : public ModeCode
{
  public:
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) {
      pumpkinColor->clear();
      return true; // switch modes immediately
    }
    char * whoIAm() { return "None Code"; }
};

class ModeRedCode : public ModeCode
{
  private:
    int counter, direction;

  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = 0;
      direction = 1;
      pC->clear();
    }
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = bumpAndLimit (counter, direction, 0, 255);
      pC->setR(counter);
      if (counter <= 0 || counter >= 255) {
        direction = -direction;
      }
      return counter <= 0;
    }
    char * whoIAm() { return "Red Code"; }
};

class ModeBlueCode : public ModeCode
{
  private:
    int counter, direction;

  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = 0;
      direction = 7;
      pC->clear();
    }
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = bumpAndLimit (counter, direction, 0, 255);
      pC->setB(counter);
      if (counter <= 0 || counter >= 255) {
        direction = -direction;
      }

      return counter <= 0;
    }
    char * whoIAm() { return "Blue Code"; }
};

class ModeGreenCode : public ModeCode
{
  private:
    int counter, direction;

  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = 0;
      direction = 2;
      pC->clear();
    }
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      counter = bumpAndLimit (counter, direction, 0, 255);
      pC->setG(counter);
      if (counter <= 0 || counter >= 255) {
        direction = -direction;
      }

      return counter <= 0;
    }
    char * whoIAm() { return "Green Code"; }
};

class ModeDimGreyCode : public ModeCode
{
  private:
    unsigned long startMillis;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
      pC->clear();
      startMillis = millis();
    }
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
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
    char * whoIAm() { return "Dim Grey"; }
};

class Pumpkin
{
  private:
    int id;

    // set in constructor
    PumpkinColor * pC;
    PumpkinParms * pP;

    ModeNoneCode modeNoneCode;
    ModeRedCode modeRedCode;
    ModeBlueCode modeBlueCode;
    ModeGreenCode modeGreenCode;
    ModeDimGreyCode modeDimGreyCode;
    
    // current state
    ModeCode * currentModeCode;
    Mode currentMode;

    // setMode was called
    bool modeWasChanged;
    Mode newMode;

    // do the next mpde change after this time
    unsigned long modeChangeMillis;

  public:
    Pumpkin(int i, PumpkinParms * _pumpkinParms)
    {
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

    int getId() {
      return id;
    }

    Mode getCurrentMode() {
      return currentMode;
    }

    PumpkinColor * getPumpkinColor() {
      return pC;
    }

    void setMode (Mode m) {
      newMode = m;
      modeWasChanged = 1;
      Serial << "setMode called for " << m << "\r\n";
    }

#undef DEBUG_PUMPKIN
    void update()
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
};

#endif
