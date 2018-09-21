#include "Pumpkin.h"

/* -------------------------- Pumpkin methods ----------------------------- */

Pumpkin::Pumpkin(int i, PumpkinParms * _pumpkinParms) {
  id = i;
  pP = _pumpkinParms;
  pC = new PumpkinColor(i);
  // Serial << "pumpkin " << i << " got pumpkinColor " << pC->getId() << "\r\n";

  currentMode = MODE_NONE;
  currentModeCode = &modeNoneCode;

  newMode = MODE_NONE;
  modeWasChanged = 1;
  lastModeChangeMillis = millis();
  howLongToStayInMode = 0;
}

int Pumpkin::getId() {
  return id;
}

Mode Pumpkin::getCurrentMode() {
  return currentMode;
}

PumpkinColor * Pumpkin::getPumpkinColor() {
  return pC;
}

void Pumpkin::setMode (Mode m) {
  newMode = m;
  modeWasChanged = 1;
  Serial << id << ": setMode called for " << m << "\r\n";
}

#define DEBUG_PUMPKIN
#undef DEBUG_PUMPKIN_COLOR

void Pumpkin::update()
{
  if (modeWasChanged) {
#ifdef DEBUG_PUMPKIN
    Serial << id << ": mode was changed. setting up for mode " << newMode << " " << modeString(newMode) << "\r\n";
#endif
    currentModeCode->finish(pP, pC);
    switch (newMode) {
      case MODE_NONE:
        currentModeCode = &modeNoneCode;
        break;
      case MODE_RED:
        currentModeCode = &modeRedCode;
        break;
      case MODE_BLUE:
        currentModeCode = &modeBlueCode;
        break;
      case MODE_GREEN:
        currentModeCode = &modeGreenCode;
        break;
      case MODE_DIMGREY:
        currentModeCode = &modeDimGreyCode;
        break;
      case MODE_RAINBOW:
        currentModeCode = &modeRainbowCode;
        break;
      default:
        Serial << id << ": unhandled newMode: " <<  currentMode << "\r\n";
        currentModeCode = &modeNoneCode;
    } 
    currentMode = newMode;
    currentModeCode->init(pP, pC);
    lastModeChangeMillis = millis();
    modeWasChanged = 0;
  }

#ifdef DEBUG_PUMPKIN_COLOR
    Serial << id << ": running: " << currentModeCode->whoIAm() << "\r\n";
#endif
  
  bool okToChange = currentModeCode->update(pP, pC);
#ifdef DEBUG_PUMPKIN_COLOR
  Serial << id << ": colors: ";
  pC->print();
  Serial.println();
#endif

  unsigned long elapsedMillis = millis() - lastModeChangeMillis;
  // Serial << id << ": elapsedmillis: " << elapsedMillis << ", howLongToStayInMode: " << howLongToStayInMode << "\r\n";
  
  if (okToChange && (elapsedMillis > howLongToStayInMode)) {
#ifdef DEBUG_PUMPKIN
    Serial << id << ": ok to change: " << okToChange << ", elapsed millis = " << elapsedMillis << ", howLongToStayInMode: " << howLongToStayInMode << "\r\n";
#endif
    ModeInterval * mi = pP->getRandomModeInterval();

#ifdef DEBUG_PUMPKIN
    Serial << id << ": randomly selected ";
    mi->print();
    Serial.println();
#endif

    modeWasChanged = 1;
    newMode = mi-> getMode();
    unsigned long tlength = mi -> getTLength();
    unsigned long variation = mi -> getVariation();
    int v = random(variation) - (variation / 2);
    if (v < 0) v = 0;
    howLongToStayInMode = tlength + v;

#ifdef DEBUG_PUMPKIN
    Serial << id << ": mode " << newMode << " " << modeString(newMode) << " was picked, next change in " << howLongToStayInMode << "\r\n";
#endif
  }

}

/* -------------------------- PumpkinParms methods ----------------------------- */

PumpkinParms::PumpkinParms(int _id) {
  id = _id;
  firstMi = NULL;
  lastMi = NULL;
  nMi = 0;
}

void PumpkinParms::add (ModeInterval * _mi) {
  if (lastMi == NULL) {
    firstMi = _mi;
    lastMi = _mi;
  } else {
    lastMi->setNext(_mi);
    lastMi = _mi;
  }
  nMi++;
}

void PumpkinParms::print() {
  Serial << "Pumpkin parms #" << id << "\r\n";
  ModeInterval * t = firstMi;
  while (t != NULL) {
    t->print();
    t = t->getNext();
    Serial.println();
  }
}

ModeInterval * PumpkinParms::getModeInterval(int _n) {
  ModeInterval * t = firstMi;
  int i = 0;
  while (t != NULL && i < _n) {
    t = t->getNext();
    i++;
  }
  return t;
}

ModeInterval * justSitThere = new ModeInterval(MODE_NONE, ULONG_MAX);


ModeInterval * PumpkinParms::getRandomModeInterval() {
  if (nMi == 0) {
    return justSitThere;
  }
  int i = random(nMi);
  return getModeInterval(i);
}

int PumpkinParms::modeIntervalCount() {
  return nMi;
}


/* -------------------------- ModeInterval methods ----------------------------- */
ModeInterval::ModeInterval (int _mode, unsigned long _tlength) {
  next = NULL;
  mode = _mode;
  tlength = _tlength;
  variation = 0;
}

ModeInterval::ModeInterval (int _mode, unsigned long _tlength, unsigned long _variation) {
  next = NULL;
  mode = _mode;
  tlength = _tlength;
  variation = _variation;
}

void ModeInterval::setNext( ModeInterval * _next) {
  next = _next;
}

ModeInterval * ModeInterval::getNext() { return next; }

int ModeInterval::getMode() { return mode; }

unsigned long ModeInterval::getTLength() { return tlength; }

unsigned long ModeInterval::getVariation() { return variation; }

void ModeInterval::print() {
  Serial << "mode interval: mode: " << mode << " " << modeString(mode) << ", tlength: " << tlength << ", variation: " << variation;
}

/* -------------------------- PumpkinColor methods ----------------------------- */

PumpkinColor::PumpkinColor(uint8_t _id) {
  id = _id + 10;
  clear();
}

int PumpkinColor::getId() { return id; }

void PumpkinColor::setR (int v) { r = v; }

void PumpkinColor::setG (int v) { g = v; }

void PumpkinColor::setB (int v) { b = v; }

void PumpkinColor::setY (int v) { y = v; }

void PumpkinColor::setUv (int v) { uv = v; }

int PumpkinColor::getR() { return r; }

int PumpkinColor::getG() { return g; }

int PumpkinColor::getB() { return b; }

int PumpkinColor::getY() { return y; }

int PumpkinColor::getUv() { return uv; }

void PumpkinColor::print() {
  Serial << "r: " << r << ", g: " << g << ", b: " << b;
}

void PumpkinColor::clear() {
  r = 0;
  g = 0;
  b = 0;
  y = 0;
  uv = 0;
}

void HsvToRgb(HsvColor hsv, RgbColor* rgb)
{
    unsigned char region, p, q, t;
    unsigned int h, s, v, remainder;

    if (hsv.s == 0)
    {
        rgb->r = hsv.v;
        rgb->g = hsv.v;
        rgb->b = hsv.v;
    }

    // converting to 16 bit to prevent overflow
    h = hsv.h;
    s = hsv.s;
    v = hsv.v;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb->r = v;
            rgb->g = t;
            rgb->b = p;
            break;
        case 1:
            rgb->r = q;
            rgb->g = v;
            rgb->b = p;
            break;
        case 2:
            rgb->r = p;
            rgb->g = v;
            rgb->b = t;
            break;
        case 3:
            rgb->r = p;
            rgb->g = q;
            rgb->b = v;
            break;
        case 4:
            rgb->r = t;
            rgb->g = p;
            rgb->b = v;
            break;
        default:
            rgb->r = v;
            rgb->g = p;
            rgb->b = q;
            break;
    }
}
