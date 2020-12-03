
#include "MatrixHandler.h"
#include "MidiNote.h"
#include "MidiChord.h"
#include <PitchToNote.h>


const byte rows = 5;
const byte columns = 13;

const byte sequenceMap[rows][columns] = {
  // from right to left 
  {1,2,3,4,5,6,7,8,9,10,11,12,13}, // 1
  {14,15,16,17,18,19,20,21,22,23,24,25}, // 2
  {43,44,45,46,47,48,49,50}, // 5
  {35,36,37,38,39,40,41,42}, // 4
  {26,27,28,29,30,31,32,33,34},  // 3
};


const IPlayable* musicKeys[rows][columns] = {
  {
    new Note(pitchE5b, 3),
    new Note(pitchA4b, 3),
    new Note(pitchC4, 3),
    new Note(pitchE4, 3),
    new Note(pitchG4, 3),
    new Note(pitchB4, 3),
    new Note(pitchD5, 3),
    new Note(pitchF5, 3),
    new Note(pitchA5, 3),
    new Note(pitchC6, 3),
    new Note(pitchE6, 3),
    new Note(pitchG6, 3),
    new Note(pitchB6, 3),
  },

  {
    new Note(pitchG5b, 3),
    new Note(pitchD4, 3),
    new Note(pitchF4, 3),
    new Note(pitchA4, 3),
    new Note(pitchC5, 3),
    new Note(pitchE5, 3),
    new Note(pitchG5, 3),
    new Note(pitchB5, 3),
    new Note(pitchD6, 3),
    new Note(pitchF6, 3),
    new Note(pitchA6, 3),
    new Note(pitchC7, 3),
  },
  
 

  {

    new Chord<4>(buildMaj7(pitchD4, 1)),
    new Note(pitchD4, 2),
    new Chord<3>(buildMaj(pitchG4)),
    new Note(pitchG3, 2),
    new Chord<3>(buildMaj(pitchC4, 2)),
    new Note(pitchC4, 2),
    new Chord<3>(buildMaj(pitchF4)),
    new Note(pitchF3, 2),
  },

  {
    new Chord<4>(buildMaj7(pitchB3, 2)),
    new Note(pitchB3, 2),
    new Chord<4>(buildMaj7(pitchE4, 1)),
    new Note(pitchE4, 2),
    new Chord<3>(buildMin(pitchA4)),
    new Note(pitchA3, 2),
    new Chord<3>(buildMin(pitchD4, 1)),
    new Note(pitchD4, 2),
  },

   {
    new Note(pitchG3b, 5),
    new Note(pitchA3, 5),
    new Note(pitchB3, 5),
    new Note(pitchC4, 5),
    new Note(pitchD4, 5),
    new Note(pitchE4, 5),
    new Note(pitchF4, 5),
    new Note(pitchG3, 5),
    new Note(pitchA3, 5),
  },
};



char data[10];
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



class MidiPressHandler : public IKeyHandler {
    void onPressed(byte x, byte y) {
      musicKeys[y][x]->update(true);
      MidiUSB.flush();
    }

    void onReleased(byte x, byte y) {
      musicKeys[y][x]->update(false);
      MidiUSB.flush();
    }
};

const MidiPressHandler midiHandler;
//const ConsoleLogHandler<columns, rows> consoleHandler;
const IKeyHandler* handler = &midiHandler;


const FixedArray<byte, rows> rowReadPins(2, 3, 4, 5, 6);
const MatrixKeyboard<columns, rows> keyboard(ShiftRegister(9, 7, 8), rowReadPins);

const MatrixHandler<columns, rows> matrixHandler(keyboard, handler);

void setup() {
  matrixHandler.onSetup();
}

void loop() {
  matrixHandler.onFrame();
}
