
#include "MatrixHandler.h"
#include "MidiNote.h"
#include "MidiChord.h"
#include <PitchToNote.h>


const IPlayable* musicKeys[] = {
  // right keyboard right most column
  new Note(pitchE5b),
  new Note(pitchA4b),
  new Note(pitchC4),
  new Note(pitchE4),
  new Note(pitchG4),
  new Note(pitchB4),
  new Note(pitchD5),
  new Note(pitchF5),
  new Note(pitchA5),

  // right keyboard second column
  new Note(pitchG5b),
  new Note(pitchD4),
  new Note(pitchF4),
  new Note(pitchA4),
  new Note(pitchC5),
  new Note(pitchE5),
  new Note(pitchG5),
  new Note(pitchB5),
  new Note(pitchD6),


  // left keyboard right most column
  new Chord<4>(buildMaj7(pitchE4, 1)),
  new Note(pitchE4),
  new Chord<3>(buildMin(pitchA4)),
  new Note(pitchA3),
  new Chord<3>(buildMin(pitchD4, 1)),
  new Note(pitchD4),

  // left keyboard left most column
  new Chord<3>(buildMaj(pitchG4)),
  new Note(pitchG3),
  new Chord<3>(buildMaj(pitchC4, 2)),
  new Note(pitchC4),
  new Chord<3>(buildMaj(pitchF4)),
  new Note(pitchF3)
};


const byte playMatrixMap[30] = {
2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 8, 1, 18, 20, 21, 22, 15, 9, 25, 19, 28, 23, 16, 0, 24, 26, 27, 29, 17

  
};


char data[10];
template<byte WIDTH, byte HEIGHT>
class ConsoleLogHandler : public IKeyHandler {
public:
    ConsoleLogHandler() {
      Serial.begin(9600);
    }
    void onPressed(byte x, byte y) {
    
      sprintf(data, "+%d ", y * WIDTH + x);
      Serial.print(data);
    }

    void onReleased(byte x, byte y) {
      sprintf(data, "-%d ", y * WIDTH + x);
      Serial.print(data);
    }
};


const byte rows = 5;
const byte columns = 6;


class MidiPressHandler : public IKeyHandler {
    void onPressed(byte x, byte y) {
      musicKeys[playMatrixMap[y*columns+x]]->update(true);
      MidiUSB.flush();
    }

    void onReleased(byte x, byte y) {
      musicKeys[playMatrixMap[y*columns+x]]->update(false);
      MidiUSB.flush();
    }
};


const MidiPressHandler midiHandler;
const ConsoleLogHandler<columns, rows> consoleHandler;
const IKeyHandler* handler = &midiHandler;


const FixedArray<byte, rows> rowReadPins(2, 3, 4, 5, 6);
const MatrixKeyboard<columns, rows> keyboard(ShiftRegister(7, 9, 8), rowReadPins);

const MatrixHandler<columns, rows> matrixHandler(keyboard, handler);

void setup() {
  matrixHandler.onSetup();
}

void loop() {
  matrixHandler.onFrame();
}
