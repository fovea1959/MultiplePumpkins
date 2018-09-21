#ifndef ModeCode_h

#define ModeCode_h

#include "PumpkinParms.h"
#include "PumpkinColor.h"

int bumpAndLimit (int v, int b, int ll, int ul);

typedef int Mode;
#define MODE_NONE 0
#define MODE_DIMGREY 1
#define MODE_RED 2
#define MODE_BLUE 3
#define MODE_GREEN 4

char * modeString(Mode _m);


// generic superclass
class ModeCode
{
  public:
    virtual void init(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
    virtual bool update(PumpkinParms * pumpkinParms, PumpkinColor * pumpkinColor);
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

#endif
