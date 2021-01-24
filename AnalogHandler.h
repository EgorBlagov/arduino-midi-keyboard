#ifndef ANALOG_HANDLER_H
#define ANALOG_HANDLER_H

class IAnalogHandler {
  public:
    virtual void onUpdate(byte value) = 0;
};

class AnalogReader {
  public:
    AnalogReader(byte pinId, IAnalogHandler* analogHandler)
      : pinId(pinId)
      , handler(analogHandler)
    {}

    void onFrame() {
      int newValue = analogRead(this->pinId);
      if (newValue != lastValue) {
        this->handler->onUpdate(newValue);
        this->lastValue = newValue;
      }
    }

  private:
    byte pinId;
    IAnalogHandler* handler;
    int lastValue;
};

#endif
