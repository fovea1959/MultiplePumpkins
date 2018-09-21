#ifndef ModeInterval_h

#define ModeInterval_h
class ModeInterval {
  private:
    ModeInterval * next;
    int tlength; // length of time this mode should be run
    int variation; // width of variation around tlength
    int mode; // what mode
  public:
    ModeInterval (int _mode, int _tlength);
    ModeInterval (int _mode, int _tlength, int _variation);
    void setNext( ModeInterval * _next);
    ModeInterval * getNext();
    int getMode();
    int getTLength();
    int getVariation();
    void print();
};
#endif
