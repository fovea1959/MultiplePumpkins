#ifndef PumpkinParms_h

#define PumpkinParms_h

#include "ModeInterval.h"

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

#endif

