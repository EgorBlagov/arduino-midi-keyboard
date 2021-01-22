
#include "MatrixHandler.h"
#include "MidiNote.h"
#include "MidiChord.h"
#include <PitchToNote.h>


const byte rows = 5;
const byte columns = 13;
char data[10];

const byte sequenceMap[rows][columns] = {
  // from right to left
  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, // 1
  {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}, // 2
  {43, 44, 45, 46, 47, 48, 49, 50}, // 5
  {35, 36, 37, 38, 39, 40, 41, 42}, // 4
  {26, 27, 28, 29, 30, 31, 32, 33, 34}, // 3
};

const channel_t MELODY_CHANNEL = 1;
const channel_t BASS_CHANNEL = 2;
const channel_t CHORD_CHANNEL = 3;
//
//
//enum ChordType {
//  ChordMaj,
//  ChordMin,
//  ChordMaj7,
//  ChordMin7,
//  ChordNone
//};
//
//struct ButtonInfo {
//  note_t mainTone;
//  channel_t mainChannel;
//
//  ChordType chordType;
//  byte chordOffset;
//
//  ButtonInfo(note_t _tone = 0, byte channel = 0, ChordType chordType = ChordNone, byte chordOffset = 0)
//    : mainTone(_tone)
//    , mainChannel(channel)
//    , chordType(chordType)
//    , chordOffset(chordOffset)
//  {}
//
//  IPlayable* create(int offset) {
//    note_t _tone = this->mainTone + offset;
//    
//
//    if (this->chordType == ChordMaj) {
//      return new Chord<3>(buildMaj(_tone, this->mainChannel, this->chordOffset));
//    }
//
//    if (this->chordType == ChordMin) {
//      return new Chord<3>(buildMin(_tone, this->mainChannel, this->chordOffset));
//    }
//
//    if (this->chordType == ChordMaj7) {
//      return new Chord<4>(buildMaj7(_tone, this->mainChannel, this->chordOffset));
//    }
//
//    if (this->chordType == ChordMin7) {
//      return new Chord<4>(buildMin7(_tone, this->mainChannel, this->chordOffset));
//    }
//
//    return new Note(_tone, this->mainChannel);
//  }
//};

// KEY is assumed to be default (starting from C4), it's easier to maintain
const note_t DEFAULT_HARMONY_KEY = pitchC4;
const note_t TARGET_KEY = pitchE4;
note_t get_note(note_t normal_note) {
  return normal_note + (int)TARGET_KEY - (int)DEFAULT_HARMONY_KEY;
}
//const ButtonInfo harmonyLayout[rows][columns] = {
//  {
//    ButtonInfo(pitchE5b, MELODY_CHANNEL),
//    ButtonInfo(pitchA4b, MELODY_CHANNEL),
//    ButtonInfo(pitchC4, MELODY_CHANNEL),
//    ButtonInfo(pitchE4, MELODY_CHANNEL),
//    ButtonInfo(pitchG4, MELODY_CHANNEL),
//    ButtonInfo(pitchB4, MELODY_CHANNEL),
//    ButtonInfo(pitchD5, MELODY_CHANNEL),
//    ButtonInfo(pitchF5, MELODY_CHANNEL),
//    ButtonInfo(pitchA5, MELODY_CHANNEL),
//    ButtonInfo(pitchC6, MELODY_CHANNEL),
//    ButtonInfo(pitchE6, MELODY_CHANNEL),
//    ButtonInfo(pitchG6, MELODY_CHANNEL),
//    ButtonInfo(pitchB6, MELODY_CHANNEL),
//  },
//
//  {
//    ButtonInfo(pitchG5b, MELODY_CHANNEL),
//    ButtonInfo(pitchD4, MELODY_CHANNEL),
//    ButtonInfo(pitchF4, MELODY_CHANNEL),
//    ButtonInfo(pitchA4, MELODY_CHANNEL),
//    ButtonInfo(pitchC5, MELODY_CHANNEL),
//    ButtonInfo(pitchE5, MELODY_CHANNEL),
//    ButtonInfo(pitchG5, MELODY_CHANNEL),
//    ButtonInfo(pitchB5, MELODY_CHANNEL),
//    ButtonInfo(pitchD6, MELODY_CHANNEL),
//    ButtonInfo(pitchF6, MELODY_CHANNEL),
//    ButtonInfo(pitchA6, MELODY_CHANNEL),
//    ButtonInfo(pitchC7, MELODY_CHANNEL),
//  },
//  {
//
//    ButtonInfo(pitchD4, CHORD_CHANNEL, ChordMaj7, 1),
//    ButtonInfo(pitchD4, BASS_CHANNEL),
//    ButtonInfo(pitchG4, CHORD_CHANNEL, ChordMaj),
//    ButtonInfo(pitchG3, BASS_CHANNEL),
//    ButtonInfo(pitchC4, CHORD_CHANNEL, ChordMaj, 2),
//    ButtonInfo(pitchC4, BASS_CHANNEL),
//    ButtonInfo(pitchF4, CHORD_CHANNEL, ChordMaj),
//    ButtonInfo(pitchF3, BASS_CHANNEL),
//  },
//  {
//    ButtonInfo(pitchB3, CHORD_CHANNEL, ChordMaj7, 2),
//    ButtonInfo(pitchB3, BASS_CHANNEL),
//    ButtonInfo(pitchE4, CHORD_CHANNEL, ChordMaj7, 1),
//    ButtonInfo(pitchE4, BASS_CHANNEL),
//    ButtonInfo(pitchA4, CHORD_CHANNEL, ChordMin),
//    ButtonInfo(pitchA3, BASS_CHANNEL),
//    ButtonInfo(pitchD4, CHORD_CHANNEL, ChordMin, 1),
//    ButtonInfo(pitchD4, BASS_CHANNEL),
//  },
//  {
//    ButtonInfo(pitchG3b, BASS_CHANNEL),
//    ButtonInfo(pitchA3, BASS_CHANNEL),
//    ButtonInfo(pitchB3, BASS_CHANNEL),
//    ButtonInfo(pitchC4, BASS_CHANNEL),
//    ButtonInfo(pitchD4, BASS_CHANNEL),
//    ButtonInfo(pitchE4, BASS_CHANNEL),
//    ButtonInfo(pitchF4, BASS_CHANNEL),
//    ButtonInfo(pitchG3, BASS_CHANNEL),
//    ButtonInfo(pitchA3, BASS_CHANNEL),
//  }
//};

const IPlayable* musicKeys[rows][columns] = {
  {
    new Note(get_note(pitchE5b), MELODY_CHANNEL),
    new Note(get_note(pitchA4b), MELODY_CHANNEL),
    new Note(get_note(pitchC4), MELODY_CHANNEL),
    new Note(get_note(pitchE4), MELODY_CHANNEL),
    new Note(get_note(pitchG4), MELODY_CHANNEL),
    new Note(get_note(pitchB4), MELODY_CHANNEL),
    new Note(get_note(pitchD5), MELODY_CHANNEL),
    new Note(get_note(pitchF5), MELODY_CHANNEL),
    new Note(get_note(pitchA5), MELODY_CHANNEL),
    new Note(get_note(pitchC6), MELODY_CHANNEL),
    new Note(get_note(pitchE6), MELODY_CHANNEL),
    new Note(get_note(pitchG6), MELODY_CHANNEL),
    new Note(get_note(pitchB6), MELODY_CHANNEL),
  },

  {
    new Note(get_note(pitchG5b), MELODY_CHANNEL),
    new Note(get_note(pitchD4), MELODY_CHANNEL),
    new Note(get_note(pitchF4), MELODY_CHANNEL),
    new Note(get_note(pitchA4), MELODY_CHANNEL),
    new Note(get_note(pitchC5), MELODY_CHANNEL),
    new Note(get_note(pitchE5), MELODY_CHANNEL),
    new Note(get_note(pitchG5), MELODY_CHANNEL),
    new Note(get_note(pitchB5), MELODY_CHANNEL),
    new Note(get_note(pitchD6), MELODY_CHANNEL),
    new Note(get_note(pitchF6), MELODY_CHANNEL),
    new Note(get_note(pitchA6), MELODY_CHANNEL),
    new Note(get_note(pitchC7), MELODY_CHANNEL),
  },



  {

    new Chord<4>(buildMaj7(get_note(pitchD4), CHORD_CHANNEL, 1)),
    new Note(get_note(pitchD4), BASS_CHANNEL),
    new Chord<3>(buildMaj(get_note(pitchG4), CHORD_CHANNEL)),
    new Note(get_note(pitchG3), BASS_CHANNEL),
    new Chord<3>(buildMaj(get_note(pitchC4), CHORD_CHANNEL, 2)),
    new Note(get_note(pitchC4), BASS_CHANNEL),
    new Chord<3>(buildMaj(get_note(pitchF4), CHORD_CHANNEL)),
    new Note(get_note(pitchF3), BASS_CHANNEL),
  },

  {
    new Chord<4>(buildMaj7(get_note(pitchB3), CHORD_CHANNEL, 2)),
    new Note(get_note(pitchB3), BASS_CHANNEL),
    new Chord<4>(buildMaj7(get_note(pitchE4), CHORD_CHANNEL, 1)),
    new Note(get_note(pitchE4), BASS_CHANNEL),
    new Chord<3>(buildMin(get_note(pitchA4), CHORD_CHANNEL)),
    new Note(get_note(pitchA3), BASS_CHANNEL),
    new Chord<3>(buildMin(get_note(pitchD4), CHORD_CHANNEL, 1)),
    new Note(get_note(pitchD4), BASS_CHANNEL),
  },

   {
    new Note(get_note(pitchG3b), BASS_CHANNEL),
    new Note(get_note(pitchA3), BASS_CHANNEL),
    new Note(get_note(pitchB3), BASS_CHANNEL),
    new Note(get_note(pitchC4), BASS_CHANNEL),
    new Note(get_note(pitchD4), BASS_CHANNEL),
    new Note(get_note(pitchE4), BASS_CHANNEL),
    new Note(get_note(pitchF4), BASS_CHANNEL),
    new Note(get_note(pitchG3), BASS_CHANNEL),
    new Note(get_note(pitchA3), BASS_CHANNEL),
  },
};

//void buildNotes(note_t key) {
//  Serial.print("begin build\n");
//
//  Serial.print("creating playable\n");
////  IPlayable*** result = new IPlayable**[rows];
//  for (int i = 0; i < rows; i++) {
//    sprintf(data, "row %d\n", i);
//    Serial.print(data);
////    result[i] = new IPlayable*[columns];
//    for (int j = 0; j < columns; j++) {
//      sprintf(data, "column %d\n", j);
//      Serial.print(data);
//      musicKeys[i][j] = harmonyLayout[i][j].create(key - DEFAULT_HARMONY_KEY);
//      Serial.print("assigning\n");
////      result[i][j] = note;
//    }
//  }
//
//  Serial.print("return result");
//}



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
