#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

class IKeyHandler {
  public:
    virtual void onPressed(byte x, byte y) = 0;
    virtual void onReleased(byte x, byte y) = 0;
};

#endif
