#ifndef DEBUG_H
#define DEBUG_H

char data[10];

const byte sequenceMap[rows][columns] = {
  // from right to left
  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, // 1
  {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}, // 2
  {43, 44, 45, 46, 47, 48, 49, 50}, // 5
  {35, 36, 37, 38, 39, 40, 41, 42}, // 4
  {26, 27, 28, 29, 30, 31, 32, 33, 34}, // 3
};

template<byte WIDTH, byte HEIGHT>
class ConsoleLogHandler : public IKeyHandler , public IAnalogHandler {
  public:
    ConsoleLogHandler(bool usb)
      : usb(usb) {
      this->init();
    }
    void onPressed(byte x, byte y) {

      sprintf(data, "+%d ", sequenceMap[y][x]);
      this->print(data);
    }

    void onReleased(byte x, byte y) {
      sprintf(data, "-%d ", sequenceMap[y][x]);
      this->print(data);
    }

    void onUpdate(byte value) {
      sprintf(data, "a%d ", value);
      this->print(data);
    }
  private:
    bool usb;
    void print(char* data) {
      if (this->usb) {
        Serial.print(data);
      } else {
        Serial1.print(data);
      }
    }

    void init() {
      if (this->usb) {
        Serial.begin(9600);
      } else {
        Serial1.begin(9600);
      }
    }
};

const ConsoleLogHandler<columns, rows> consoleHandler(false);
#endif
