#ifndef PumpkinLighter_h

#define PumpkinLighter_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG_LIGHTERS

class PumpkinLighter {
  private:
    int rgb_index, y_index, uv_index;
    Adafruit_NeoPixel * string;

  public:
    PumpkinLighter (Adafruit_NeoPixel * _string, int _rgb_index, int _y_index, int _uv_index) {
      string = _string;
      rgb_index = _rgb_index;
      y_index = _y_index;
      uv_index = _uv_index;
    }
    void update(PumpkinColor * c) {
      Serial.println ("update called");
#ifdef DEBUG_LIGHTERS      
      Serial.print ("updating Neopixels ");
      Serial.print ((int) c);
      Serial.print ("r: ");
      Serial.print (c->r);
      Serial.print (", g: ");
      Serial.print (c->g);
      Serial.print (", b: ");
      Serial.print (c->b);
      Serial.println();
#endif
      string->setPixelColor (rgb_index, c->r, c->g, c->b);
      string->setPixelColor (y_index, c->y, c->y, c->y);
      string->setPixelColor (uv_index, c->uv, c->uv, c->uv);
    }
};

#endif
