#ifndef BUTTON_H
#define BUTTON_H
    
class Button {
  public:
    Button():
      Button(0)
    {}

    Button(int pinId, bool inversed = false, int delay = 50, bool pullup = false)
      : pinId(pinId)
      , lastPress(0)
      , delay(delay)
    {
      this->buttonState.state(false);
      this->buttonState.prevState(false);
      this->buttonState.inversed(inversed);
    }

    void onSetup(byte mode=INPUT) {
      pinMode(this->pinId, mode);
    }

    void onFrame() {
      const bool currentState = this->readButton();
      this->buttonState.prevState(this->buttonState.state());
      if (this->delay == 0) {
        this->buttonState.state(currentState);
      } else if (currentState != this->buttonState.state()) {
        const auto now = millis();
        if (now - this->lastPress > this->delay) {
          this->buttonState.state(currentState);
          this->lastPress = now;
        }
      }
    }

    bool isPressed() {
      return this->buttonState.state();
    }

    bool justPressed() {
      return this->buttonState.state() && !this->buttonState.prevState();
    }

    bool justReleased() {
      return !this->buttonState.state() && this->buttonState.prevState();
    }

  private:
    class ButtonState {
        enum class DATA_POS {
          STATE = 0,
          PREVSTATE,
          INVERSED
        };

      public:
        bool state() {
          return this->get(DATA_POS::STATE);
        }

        void state(bool value) {
          this->set(DATA_POS::STATE, value);
        }

        bool prevState() {
          return this->get(DATA_POS::PREVSTATE);
        }

        void prevState(bool value) {
          return this->set(DATA_POS::PREVSTATE, value);
        }

        bool inversed() {
          return this->get(DATA_POS::INVERSED);
        }

        void inversed(bool value) {
          this->set(DATA_POS::INVERSED, value);
        }
      private:

        bool get(DATA_POS dataPos) {
          return bitRead(this->data, (byte)dataPos);
        }

        void set(DATA_POS dataPos, bool value) {
          if (value) {
            bitSet(this->data, (byte)dataPos);
          } else {
            bitClear(this->data, (byte)dataPos);
          }
        }

        byte data;
    };

    ButtonState buttonState;
    byte pinId;
    unsigned long lastPress;
    word delay;

    bool readButton() {
      return this->buttonState.inversed() ^ digitalRead(this->pinId); // XOR
    }
};

#endif
