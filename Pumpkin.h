#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

#define DEBUG_ON

#include "Debug.h"

class ModeInterval {
  private:
    ModeInterval * next;
    int tlength; // length of time this mode should be run
    int variation; // width of variation around tlength
    int mode; // what mode
  public:
    ModeInterval (int _mode, int _tlength) {
      next = NULL;
      mode = _mode;
      tlength = _tlength;
      variation = 0;
    }
    ModeInterval (int _mode, int _tlength, int _variation) {
      next = NULL;
      mode = _mode;
      tlength = _tlength;
      variation = _variation;
    }
    void setNext( ModeInterval * _next) {
      next = _next;
    }
    ModeInterval * getNext() {
      return next;
    }
    int getMode() {
      return mode;
    }
    int getTLength() {
      return tlength;
    }
    int getVariation() {
      return variation;
    }
    void print() {
      Serial << "mode interval: " << mode << ", tlength: " << tlength << ", variation: " << variation;
    }
};

class PumpkinParms {
  private:
    int id;
    ModeInterval * firstMi; // beginning of LL
    ModeInterval * lastMi; // end of LL
    int nMi;
  public:
    PumpkinParms(int _id) {
      id = _id;
      firstMi = NULL;
      lastMi = NULL;
      nMi = 0;
    }
    void add (ModeInterval * _mi) {
      if (lastMi == NULL) {
        firstMi = _mi;
        lastMi = _mi;
      } else {
        lastMi->setNext(_mi);
        lastMi = _mi;
      }
      nMi++;
    }
    void print() {
      Serial << "Pumpkin parms #" << id << "\r\n";
      ModeInterval * t = firstMi;
      while (t != NULL) {
        t->print();
        t = t->getNext();
        Serial.println();
      }
    }
    ModeInterval * getModeInterval(int _n) {
      ModeInterval * t = firstMi;
      int i = 0;
      while (t != NULL && i < _n) {
        t = t->getNext();
        i++;
      }
      return t;
    }
    ModeInterval * getRandomModeInterval() {
      int i = random(nMi);
      return getModeInterval(i);
    }
    int modeIntervalCount() {
      return nMi;
    }
};

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
//#define MODE_WHITE 4
#define MODE_TOOHIGH 5

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
    virtual void printWhoIAm() { Serial.println ("superclass"); };
};

class ModeNoneCode : public ModeCode
{
  public:
    bool loop(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor) {
      pumpkinColor->clear();
      return true; // switch modes
    }
    void printWhoIAm() { Serial.println ("None"); };
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
    void printWhoIAm() { Serial.println ("Red Code"); };
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
    void printWhoIAm() { Serial.println ("Blue Code"); };
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
    void printWhoIAm() { Serial.println ("Green Code"); };
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
    void printWhoIAm() { Serial.println ("DimGrey Code"); };
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

    void update()
    {
      unsigned long currentMillis = millis();

      if (modeWasChanged) {
        Serial << "mode was changed. setting up for mode " << newMode << " " << modeString(newMode) << "\r\n";
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

      if (false) {
        Serial << "running: ";
        currentModeCode->printWhoIAm();
      }
      
      bool okToChange = currentModeCode->update(pP, pC);

      if (false) {
        Serial << "colors: ";
        pC->print();
        Serial.println();
      }
      
      if (okToChange && (currentMillis > modeChangeMillis)) {
        Serial << "ok to change: " << okToChange << ", current millis = " << currentMillis << ", mode change millis: " << modeChangeMillis << "\r\n";
        ModeInterval * mi = pP->getRandomModeInterval();
        
        Serial << "got modeInterval ";
        mi->print();
        Serial.println();

        modeWasChanged = 1;
        newMode = mi-> getMode();
        int tlength = mi -> getTLength();
        int variation = mi -> getVariation();
        modeChangeMillis = currentMillis + tlength + random(variation);
        Serial << "mode " << newMode << modeString(newMode) << " was picked, next change at " << modeChangeMillis << "\r\n";
      }

    }
};

#endif
