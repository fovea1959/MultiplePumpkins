#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

#define DEBUG_ON

#include "Debug.h"

#include "PumpkinParms.h"
#include "PumpkinColor.h"
#include "ModeInterval.h"
#include "ModeCode.h"

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

    // do the next mode change after this time
    unsigned long modeChangeMillis;

  public:
    Pumpkin(int i, PumpkinParms * _pumpkinParms);
    int getId();
    Mode getCurrentMode();
    PumpkinColor * getPumpkinColor();
    void setMode (Mode m);
    void update();

};

#endif
