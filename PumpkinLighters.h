#ifndef PumpkinLighter_h

#define PumpkinLighter_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Debug.h"

// #define DEBUG_LIGHTERS

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
#ifdef DEBUG_LIGHTERS      
      Serial << "updating Neopixels " << ((int) c) << " r: " << c->r << ", g: " << c->g <<", b: " << c->b << "\r\n";
#endif
      string->setPixelColor (rgb_index, c->r, c->g, c->b);
      if (y_index >= 0) {
        string->setPixelColor (y_index, c->y, c->y, c->y);
      }
      if (uv_index >= 0) {
        string->setPixelColor (uv_index, c->uv, c->uv, c->uv);
      }
    }
};

#endif
