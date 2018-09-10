#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

#define DEBUG_ON

#include "Debug.h"


typedef int Mode;
#define MODE_NONE 0
#define MODE_RED 1
#define MODE_BLUE 2
#define MODE_GREEN 3
#define MODE_TOOHIGH 4


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
      Debug << "r: " << r << ", g: " << g << ", b: " << b;
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
      Serial << "pumpkin " << i << " got pumpkinColor " << pC->getId() << "\r\n";
    }

    void printId() {
      Serial << "Pumpkin " << id;
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

        // should create a intramodeStarttime and initialize it here

        // we just switched into this mode
        switch (currentMode) {
          case MODE_BLUE:
          case MODE_RED:
          case MODE_GREEN:
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
        case MODE_RED:
        case MODE_GREEN:
          pC -> clear();

          switch (currentMode) {
            case MODE_BLUE:
              pC -> b = intramodeCounter;
              intramodeCounter = intramodeCounter + intramodeDirection;
              break;
            case MODE_RED:
              pC -> r = intramodeCounter;
              intramodeCounter = intramodeCounter + intramodeDirection * 5;
              break;
            case MODE_GREEN:
              pC -> g = intramodeCounter;
              intramodeCounter = intramodeCounter + intramodeDirection * 7;
              break;
          }

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
        // need to look at current enabled modes and do a setMode on
        // on the next eligible one. if none are eligible, then use MODE_NONE

      }
    }
};

#endif
