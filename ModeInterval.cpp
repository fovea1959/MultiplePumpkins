#include <Arduino.h>
#include "ModeInterval.h"
#include "Debug.h"

ModeInterval::ModeInterval (int _mode, int _tlength) {
      next = NULL;
      mode = _mode;
      tlength = _tlength;
      variation = 0;
    }
    ModeInterval::ModeInterval (int _mode, int _tlength, int _variation) {
      next = NULL;
      mode = _mode;
      tlength = _tlength;
      variation = _variation;
    }
    void ModeInterval::setNext( ModeInterval * _next) {
      next = _next;
    }
    ModeInterval * ModeInterval::getNext() {
      return next;
    }
    int ModeInterval::getMode() {
      return mode;
    }
    int ModeInterval::getTLength() {
      return tlength;
    }
    int ModeInterval::getVariation() {
      return variation;
    }
    void ModeInterval::print() {
      Serial << "mode interval: " << mode << ", tlength: " << tlength << ", variation: " << variation;
    }

