#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

class IEncoderHandler {
  public:
    virtual void onIncrease() = 0;
    virtual void onDecrease() = 0;
    virtual void onSetup() = 0;
};

class EncoderReader {
  public:
    EncoderReader(byte pinA, byte pinB, IEncoderHandler* encoderHandler, byte reverse = false)
      : pinA(pinA)
      , pinB(pinB)
      , handler(encoderHandler)
      , reverse(reverse)
    {}

    void onSetup() {
      pinMode(this->pinA, INPUT_PULLUP);
      pinMode(this->pinB, INPUT_PULLUP);
      lastValue = digitalRead(pinA);
      handler->onSetup();
      
    }

    void onFrame() {
      handleScroll();
    }

  private:
    void handleScroll() {
      byte newValue = digitalRead(this->pinA);
      if (newValue != lastValue) {
        if (newValue && !lastValue) {
          if (digitalRead(this->pinB) ^ reverse) {
            this->handler->onIncrease();
          } else {
            this->handler->onDecrease();
          }
        }
        this->lastValue = newValue;
      }
    }

    byte pinA;
    byte pinB;
    IEncoderHandler* handler;
    byte lastValue;
    byte reverse;
};

#endif
