#ifndef PumpkinLighter_h

#define PumpkinLighter_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Pumpkin.h"

// #define DEBUG_LIGHTERS

class PumpkinLighter {
  private:
    int rgb_index_l, rgb_index_h, y_index, uv_index;
    Adafruit_NeoPixel * string;

  public:
    PumpkinLighter (Adafruit_NeoPixel * _string, int _rgb_index, int _y_index, int _uv_index) {
      string = _string;
      rgb_index_l = _rgb_index;
      rgb_index_h = _rgb_index;
      y_index = _y_index;
      uv_index = _uv_index;
    }
    PumpkinLighter (Adafruit_NeoPixel * _string, int _rgb_index_l, int _count, int _y_index, int _uv_index) {
      string = _string;
      rgb_index_l = _rgb_index_l;
      rgb_index_h = _rgb_index_l + _count - 1;
      y_index = _y_index;
      uv_index = _uv_index;
    }
    void update(PumpkinColor * c) {
#ifdef DEBUG_LIGHTERS      
      Serial << "updating Neopixels " << ((int) c) << " r: " << c->r << ", g: " << c->g <<", b: " << c->b << "\r\n";
#endif
      if (rgb_index_l >= 0) {
        for (int i = rgb_index_l; i <= rgb_index_h; i++) {
          string->setPixelColor (i, c->getR(), c->getG(), c->getB());
        }
      }
      if (y_index >= 0) {
        string->setPixelColor (y_index, c->getY(), c->getY(), c->getY());
      }
      if (uv_index >= 0) {
        string->setPixelColor (uv_index, c->getUv(), c->getUv(), c->getUv());
      }
    }
};

#endif
