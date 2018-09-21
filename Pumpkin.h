#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <limits.h>
#include "elapsedMillis.h"

#define DEBUG_ON

// From https://playground.arduino.cc/Main/StreamingOutput
template<class T> inline Print& operator<<(Print &obj, T arg)
{
  obj.print(arg);
  return obj;
}

#ifdef DEBUG_ON
# define Debug Serial
#else
class DummyStream {
  public:
    template<class T> DummyStream& operator<<(T arg)
    {
      (void) arg;
      return *this;
    }
} Debug;
#endif

// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
// see Shonn's answer
typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;


void HsvToRgb(HsvColor hsv, RgbColor* rgb);


class ModeInterval {
  private:
    ModeInterval * next;
    unsigned long tlength; // length of time this mode should be run
    unsigned long variation; // width of variation around tlength
    int mode; // what mode
  public:
    ModeInterval (int _mode, unsigned long _tlength);
    ModeInterval (int _mode, unsigned long _tlength, unsigned long _variation);
    void setNext( ModeInterval * _next);
    ModeInterval * getNext() ;
    int getMode();
    unsigned long getTLength();
    unsigned long getVariation();
    void print();
};

class PumpkinParms {
  private:
    int id;
    ModeInterval * firstMi; // beginning of LL
    ModeInterval * lastMi; // end of LL
    int nMi;
  public:
    PumpkinParms(int _id);
    void add (ModeInterval * _mi);
    void print();
    ModeInterval * getModeInterval(int _n);
    ModeInterval * getRandomModeInterval();
    int modeIntervalCount();
};

class PumpkinColor {
  private:
    int id, r, g, b, y, uv;
  public:
    PumpkinColor(uint8_t _id);
    int getId();
    void setR (int v);
    void setG (int v);
    void setB (int v);
    void setY (int v);
    void setUv (int v);
    int getR();
    int getG();
    int getB();
    int getY();
    int getUv();
    void print();
    void clear();
};

typedef int Mode;
#define MODE_NONE 0
#define MODE_DIMGREY 1
#define MODE_RED 2
#define MODE_BLUE 3
#define MODE_GREEN 4
#define MODE_RAINBOW 5

char * modeString(Mode _m);

// generic superclass
class ModeCode
{
  public:
    virtual void init(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
    virtual bool update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
    virtual void requestModeEnd();
    virtual void finish(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
    virtual char * whoIAm();
};

class ModeNoneCode : public ModeCode
{
  public:
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
    char * whoIAm();
};

class ModeRedCode : public ModeCode
{
  private:
    int counter, direction;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    char * whoIAm();
};

class ModeBlueCode : public ModeCode
{
  private:
    int counter, direction;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    char * whoIAm();
};

class ModeGreenCode : public ModeCode
{
  private:
    int counter, direction;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    char * whoIAm();
};

class ModeDimGreyCode : public ModeCode
{
  private:
    unsigned long startMillis;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    char * whoIAm();
};

class ModeRainbowCode : public ModeCode
{
  private:
    int state;
    elapsedMillis t;
    long t_wait;
    HsvColor c0;
    HsvColor c1;
  public:
    void init(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    bool update(PumpkinParms * pumpkinParms, PumpkinColor * pC);
    char * whoIAm();
};

class Pumpkin
{
  private:
    int id;

    // set in constructor
    PumpkinParms * pP;
    
    PumpkinColor * pC;

    // it would be cool to just create this when needed
    ModeNoneCode modeNoneCode;
    ModeRedCode modeRedCode;
    ModeBlueCode modeBlueCode;
    ModeGreenCode modeGreenCode;
    ModeDimGreyCode modeDimGreyCode;
    ModeRainbowCode modeRainbowCode;
    
    // current state
    ModeCode * currentModeCode;
    Mode currentMode;

    // setMode was called
    bool modeWasChanged;
    Mode newMode;

    // do the next mode change after this time
    unsigned long lastModeChangeMillis;
    unsigned long howLongToStayInMode;

  public:
    Pumpkin(int i, PumpkinParms * _pumpkinParms);
    int getId();
    Mode getCurrentMode();
    PumpkinColor * getPumpkinColor();
    void setMode (Mode m);
    void update();
};

#endif
