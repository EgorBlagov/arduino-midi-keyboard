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
      , delay(200)
    {}

    void onFrame() {
      const auto now = millis();
        if (now - this->lastPress > this->delay) {
          int newValue = analogRead(this->pinId);
          byte newByteValue = newValue / 4;
          if (newByteValue != lastValue) {
            this->handler->onUpdate(newByteValue);
            this->lastValue = newByteValue;
          }
          this->lastPress = now;
        }      
    }

  private:
    byte pinId;
    IAnalogHandler* handler;
    byte lastValue;
    unsigned long lastPress;
    word delay;
    
};

#endif
