#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

typedef int Mode;
#define MODE_NONE 0
#define MODE_RED 1
#define MODE_BLUE 2
#define MODE_TOOHIGH 3


class PumpkinColor
{
  public:
    int id, r, g, b, y, uv;
    PumpkinColor(uint8_t _id) {
      id = _id + 10;
    }
    int getId() {
      return id;
    }
    void print() {
      Serial.print ("r: ");
      Serial.print (r);
      Serial.print (", g: ");
      Serial.print (g);
      Serial.print (", b: ");
      Serial.print (b);
    }
    void clear() {
      r = 0;
      g = 0;
      b = 0;
      y = 0;
      uv = 0;
    }

};

class PumpkinParms
{
  private:
  public:
    PumpkinParms()
    {
    }
};

class Pumpkin
{
  private:
    int id;

    int intramodeCounter;
    int intramodeDirection;
    
    PumpkinColor * pC;
    PumpkinParms * pP;
    Mode currentMode = MODE_NONE;
    bool newMode = 0;
  public:
    Pumpkin(int i, PumpkinParms * _pumpkinParms)
    {
      id = i;
      pP = _pumpkinParms;
      pC = new PumpkinColor(i);
      Serial.print ("pumpkin ");
      Serial.print (i);
      Serial.print (" got pumpkinColor ");
      Serial.println (pC->getId());
    }

    void printId() {
      Serial.print ("Pumpkin ");
      Serial.print (id);
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
      currentMode = m;
      newMode = 1;
      Serial.print ("setMode called for ");
      Serial.print (id);
      Serial.print (", value = ");
      Serial.println(m);

    }

    void update()
    {
      bool changeMode = 0;

      if (newMode) {
        // we just switched into this mode
        switch (currentMode) {
          case MODE_BLUE:
          case MODE_RED:
            intramodeCounter = 0;
            intramodeDirection = 1;
            break;
        }
        newMode = 0;

      }

      switch (currentMode) {
        case MODE_NONE:
          changeMode = 1;
          break;
        case MODE_BLUE:
          pC -> clear();
          pC -> b = intramodeCounter;

          intramodeCounter = intramodeCounter + intramodeDirection;
          if (intramodeCounter >= 255) {
            intramodeCounter = 255;
            intramodeDirection = -1;
          } else if (intramodeCounter < 0) {
            intramodeCounter = 0;
            intramodeDirection = 1;
          }
          break;
        case MODE_RED:
          pC -> clear();
          pC -> r = intramodeCounter;

          intramodeCounter = intramodeCounter + intramodeDirection*2;
          if (intramodeCounter >= 255) {
            intramodeCounter = 255;
            intramodeDirection = -1;
          } else if (intramodeCounter < 0) {
            intramodeCounter = 0;
            intramodeDirection = 1;
          }
break;
        default:
          break;
      }

      if (changeMode) {

      }
    }
};

#endif
