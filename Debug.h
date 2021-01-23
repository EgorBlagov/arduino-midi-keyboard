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
class ConsoleLogHandler : public IKeyHandler {
  public:
    ConsoleLogHandler() {
      Serial.begin(9600);
    }
    void onPressed(byte x, byte y) {

      sprintf(data, "+%d ", sequenceMap[y][x]);
      Serial.print(data);
    }

    void onReleased(byte x, byte y) {
      sprintf(data, "-%d ", sequenceMap[y][x]);
      Serial.print(data);
    }
};

const ConsoleLogHandler<columns, rows> consoleHandler;
#endif
