#ifndef Pumpkin_h

#define Pumpkin_h

#include <Arduino.h>

class PumpkinColor
{
  public:
    uint8_t id, r, g, b, y, uv;
    PumpkinColor(uint8_t _id) {
      id = _id + 10;
    }
    int getId() {
      return id;
    }

};

/*

class PumpkinMode
{
  private:
    PumpkinColor *pumpkinColor;
  public:
    PumpkinMode(PumpkinColor &p)
    {
      *pumpkinColor = p;
    }



    virtual void Init()
    {

    }

    virtual void Update()
    {

    }
};

*/ 

class Pumpkin
{
  private:
    int id;
    PumpkinColor *pumpkinColor;
  public:
    Pumpkin(int i, PumpkinColor* p)
    {
      id = i;
      pumpkinColor = p;
      Serial.print ("pumpkin ");
      Serial.print (i);
      Serial.print (" got pumpkinColor ");
      Serial.println (p->getId());
    }

    int getId() {
      return id;
    }

    PumpkinColor* getPumpkinColor() {
      return pumpkinColor;
    }

    void Update()
    {
    }
};

#endif
