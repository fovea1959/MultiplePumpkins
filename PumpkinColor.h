#ifndef PumpkinColor_h

#define PumpkinColor_h

class PumpkinColor
{
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

#endif
