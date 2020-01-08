#ifndef MATRIX_HANDLER_H
#define MATRIX_HANDLER_H

#include "MatrixKeyboard.h"
#include "KeyHandler.h"

template<byte WIDTH, byte HEIGHT>
class MatrixHandler {
  public:
    MatrixHandler(MatrixKeyboard<WIDTH, HEIGHT> keyboard, IKeyHandler* keyHandler)
      : keyboard(keyboard)
      , handler(keyHandler)
    {}

    void onSetup() {
      this->keyboard.onSetup();
    }

    void onFrame() {
      this->keyboard.onFrame();
      for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
          if (this->keyboard.getButton(x, y).justPressed()) {
            this->handler->onPressed(x, y);
          } else if (this->keyboard.getButton(x, y).justReleased()) {
            this->handler->onReleased(x, y);
          }
        }
      }
    }

  private:
    MatrixKeyboard<WIDTH, HEIGHT> keyboard;
    IKeyHandler* handler;
};

#endif
