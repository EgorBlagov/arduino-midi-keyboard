#ifndef MATRIX_KEYBOARD_H
#define MATRIX_KEYBOARD_H

#include "FixedArray.h"
#include "ShiftRegister.h"
#include "Button.h"

template<byte WIDTH, byte HEIGHT>
class MatrixKeyboard {
  public:
    MatrixKeyboard(const ShiftRegister& columnWriteRegister, const FixedArray<byte, HEIGHT>& rowReadPins)
      : columnWriteRegister(columnWriteRegister)
      , rowReadPins(rowReadPins)
    {
      for (byte x = 0; x < WIDTH; x++) {
        for (byte y = 0; y < HEIGHT; y++) {
          this->buttons[x][y] = Button(this->rowReadPins[y], true);
        }
      }
    }

    void onSetup() {
      for (byte i = 0; i < HEIGHT; i++) {
        pinMode(this->rowReadPins[i], INPUT);
      }

      this->columnWriteRegister.onSetup();
      this->columnWriteRegister.putBit(true, WIDTH);
      this->columnWriteRegister.publish();
    }

    void onFrame() {
      this->columnWriteRegister.putBit(false);
      this->columnWriteRegister.publish();

      for (byte x = 0; x < WIDTH; x++) {
        for (byte y = 0; y < HEIGHT; y++) {
          this->buttons[x][y].onFrame();
        }
        this->columnWriteRegister.putBit(true);
        this->columnWriteRegister.publish();
      }
    }

    Button getButton(byte x, byte y) {
      return this->buttons[x][y];
    }

  private:
    Button buttons[WIDTH][HEIGHT];
    FixedArray<byte, HEIGHT> rowReadPins;
    ShiftRegister columnWriteRegister;
};

#endif
