#include <Arduino.h>
#include "Pumpkin.h"

#define S_MOVING 0
#define S_HOLDING 1

#if DW_DEBUG == 0
#define T_HOLDING 5000
#define T_MOVING 5000
#else
#define T_HOLDING 1000
#define T_MOVING 5000
#endif

void rainbow_randomColor(HsvColor* hsv) {
  hsv->h = random(255);
  hsv->s = 255; // 200+random(55);
  hsv->v = 63; // +random(192);
}

void rainbow_calculate (HsvColor c0, HsvColor c1, long t, long t_wait, HsvColor* result) {
 int32_t h1_calc;

 int32_t h0 = c0.h;
 int32_t h1 = c1.h;
 int32_t s0 = c0.s;
 int32_t s1 = c1.s;
 int32_t v0 = c0.v;
 int32_t v1 = c1.v;

 int32_t h_delta = h1 - h0;
 if (abs(h_delta) <= 127) {
  h1_calc = h1;
 } else {
  if (h_delta < 0) {
   h1_calc = h1 + 256;
  } else {
   h1_calc = h1 - 256;
  }
 }

 int32_t h = h0 + ((h1_calc - h0) * (t) / (t_wait));
 h = h & 0xff;
 int32_t s = s0 + ((s1 - s0) * (t) / (t_wait));
 int32_t v = v0 + ((v1 - v0) * (t) / (t_wait));
 result->h = h;
 result->s = s;
 result->v = v;
}

void ModeRainbowCode::init(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  pC->clear();
  state = S_HOLDING;
  rainbow_randomColor(&c0);
  c1 = c0;
  t = elapsedMillis();
  t_wait = T_HOLDING;
}

void rainbow_setis(HsvColor hsv, PumpkinColor * pC) {
  RgbColor rgb;
  HsvToRgb(hsv, &rgb);
  pC->setR(rgb.r);
  pC->setG(rgb.g);
  pC->setB(rgb.b);
}

bool ModeRainbowCode::update(PumpkinParms * pumpkinParms, PumpkinColor * pC) {
  if (state == S_HOLDING) {
    if (t <= t_wait) {
      rainbow_setis(c1, pC);
    } else {
      state = S_MOVING;
      c0 = c1;
      rainbow_randomColor(&c1);
      //setWas(c0);
      //setWillBe(c1);
      rainbow_setis(c0, pC);
      t = elapsedMillis();
      t_wait = T_MOVING;
    }
  } else if (state == S_MOVING) {
    if (t <= t_wait) {
      HsvColor c;
      rainbow_calculate (c0, c1, t, t_wait, &c);
      rainbow_setis(c, pC);
    } else {
      state = S_HOLDING;
      //setWas(c1);
      rainbow_setis(c1, pC);
      t = elapsedMillis();
      t_wait = T_HOLDING;
    }
  }  
}

char * ModeRainbowCode::whoIAm() { return "Rainbow"; }

