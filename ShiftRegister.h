#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

class ShiftRegister {
  public:
    ShiftRegister(byte shcpPin, byte dsPin, byte stcpPin)
      : shcpPin(shcpPin)
      , dsPin(dsPin)
      , stcpPin(stcpPin)
    {}

    void onSetup() {
      pinMode(this->shcpPin, OUTPUT);
      pinMode(this->dsPin, OUTPUT);
      pinMode(this->stcpPin, OUTPUT);

      digitalWrite(this->shcpPin, LOW);
      digitalWrite(this->dsPin, LOW);
      digitalWrite(this->stcpPin, LOW);
    }

    void publish() {
      digitalWrite(this->stcpPin, HIGH);
      digitalWrite(this->stcpPin, LOW);
    }

    void putBit(bool isHigh, byte times = 1) {
      for (byte i = 0; i < times; i++) {
        digitalWrite(this->dsPin, isHigh ? HIGH : LOW);
        this->tick();
      }
    }

  private:
    void tick() {
      digitalWrite(this->shcpPin, HIGH);
      digitalWrite(this->shcpPin, LOW);
    }

    byte shcpPin;
    byte dsPin;
    byte stcpPin;
};

#endif
