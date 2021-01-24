#include "Common.h"
#include "MatrixHandler.h"
#include "AnalogHandler.h"
#include "Chord.h"
#include <PitchToNote.h>

const byte rows = 5;
const byte columns = 13;

const channel_t MELODY_CHANNEL = 1;
const channel_t BASS_CHANNEL = 2;
const channel_t CHORD_CHANNEL = 3;
const channel_t CONTROL_CHANNEL = 4;

// KEY is assumed to be default (starting from C4), it's easier to maintain
const note_t DEFAULT_HARMONY_KEY = pitchC4;
const note_t MAX_KEY = pitchD5b;
const note_t MIN_KEY = pitchC2;

note_t TARGET_KEY = pitchC4;

const byte VOLUME_ID = 1;

//#define DEBUG
#ifdef DEBUG
#include "Debug.h"
#endif

const byte DEFAULT_VELOCITY = 64;
void noteOn(note_t note, channel_t channel, velocity_t velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, note, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(note_t note, channel_t channel) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, note, 0};
  MidiUSB.sendMIDI(noteOff);
}

void setController(channel_t channel, byte id, byte volume) {
  midiEventPacket_t setVolume = {0x0B, 0xB0 | channel, id, volume};
  MidiUSB.sendMIDI(setVolume);
}


note_t get_note(note_t normal_note) {
  return normal_note + (int)TARGET_KEY - (int)DEFAULT_HARMONY_KEY;
}

void setNewKey(note_t targetKey) {
  if (targetKey > MAX_KEY) {
    TARGET_KEY = MAX_KEY;
  } else if (targetKey < MIN_KEY) {
    TARGET_KEY = MIN_KEY;
  } else {
    TARGET_KEY = targetKey;
  }
}

class ButtonInfo {
  public:
    enum ButtonType {
      ButtonChordMaj,
      ButtonChordMin,
      ButtonChordMaj7,
      ButtonChordMin7,
      ButtonNote,
    };

    ButtonInfo(note_t _tone = 0, byte channel = 0, ButtonType buttonType = ButtonNote, byte chordOffset = 0)
      : mainChannel(channel)
    {
      switch (buttonType) {
        case ButtonNote:
          this->count = 1;
          notes[0] = _tone;
          break;
        case ButtonChordMaj:
          this->count = 3;
          buildFromStructure(_tone, majStructure, chordOffset).emplace(notes);
          break;
        case ButtonChordMin:
          this->count = 3;
          buildFromStructure(_tone, minStructure, chordOffset).emplace(notes);
          break;
        case ButtonChordMaj7:
          this->count = 4;
          buildFromStructure(_tone, maj7Structure, chordOffset).emplace(notes);
          break;
        case ButtonChordMin7:
          this->count = 4;
          buildFromStructure(_tone, min7Structure, chordOffset).emplace(notes);
          break;
      }
    }

    void update(bool pressed) {
      if (pressed) {
        for (int i = 0; i < this->count; i++) {
          noteOn(get_note(this->notes[i]), this->mainChannel, DEFAULT_VELOCITY);
        }
      } else {
        for (int i = 0; i < this->count; i++) {
          noteOff(get_note(this->notes[i]), this->mainChannel);
        }
      }
    }

  private:
    note_t notes[4];
    channel_t mainChannel;
    byte count;

};

const ButtonInfo musicKeys[rows][columns] = {
  {
    ButtonInfo(pitchE5b, MELODY_CHANNEL),
    ButtonInfo(pitchA4b, MELODY_CHANNEL),
    ButtonInfo(pitchC4, MELODY_CHANNEL),
    ButtonInfo(pitchE4, MELODY_CHANNEL),
    ButtonInfo(pitchG4, MELODY_CHANNEL),
    ButtonInfo(pitchB4, MELODY_CHANNEL),
    ButtonInfo(pitchD5, MELODY_CHANNEL),
    ButtonInfo(pitchF5, MELODY_CHANNEL),
    ButtonInfo(pitchA5, MELODY_CHANNEL),
    ButtonInfo(pitchC6, MELODY_CHANNEL),
    ButtonInfo(pitchE6, MELODY_CHANNEL),
    ButtonInfo(pitchG6, MELODY_CHANNEL),
    ButtonInfo(pitchB6, MELODY_CHANNEL),
  },

  {
    ButtonInfo(pitchG5b, MELODY_CHANNEL),
    ButtonInfo(pitchD4, MELODY_CHANNEL),
    ButtonInfo(pitchF4, MELODY_CHANNEL),
    ButtonInfo(pitchA4, MELODY_CHANNEL),
    ButtonInfo(pitchC5, MELODY_CHANNEL),
    ButtonInfo(pitchE5, MELODY_CHANNEL),
    ButtonInfo(pitchG5, MELODY_CHANNEL),
    ButtonInfo(pitchB5, MELODY_CHANNEL),
    ButtonInfo(pitchD6, MELODY_CHANNEL),
    ButtonInfo(pitchF6, MELODY_CHANNEL),
    ButtonInfo(pitchA6, MELODY_CHANNEL),
    ButtonInfo(pitchC7, MELODY_CHANNEL),
  },
  {

    ButtonInfo(pitchD3, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj7, 1),
    ButtonInfo(pitchD3, BASS_CHANNEL),
    ButtonInfo(pitchG3, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj),
    ButtonInfo(pitchG2, BASS_CHANNEL),
    ButtonInfo(pitchC3, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj, 2),
    ButtonInfo(pitchC3, BASS_CHANNEL),
    ButtonInfo(pitchF3, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj),
    ButtonInfo(pitchF2, BASS_CHANNEL),
  },
  {
    ButtonInfo(pitchB2, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj7, 2),
    ButtonInfo(pitchB2, BASS_CHANNEL),
    ButtonInfo(pitchE3, CHORD_CHANNEL, ButtonInfo::ButtonChordMaj7, 1),
    ButtonInfo(pitchE3, BASS_CHANNEL),
    ButtonInfo(pitchA3, CHORD_CHANNEL, ButtonInfo::ButtonChordMin),
    ButtonInfo(pitchA2, BASS_CHANNEL),
    ButtonInfo(pitchD3, CHORD_CHANNEL, ButtonInfo::ButtonChordMin, 1),
    ButtonInfo(pitchD3, BASS_CHANNEL),
  },
  {
    ButtonInfo(pitchG2b, BASS_CHANNEL),
    ButtonInfo(pitchA2, BASS_CHANNEL),
    ButtonInfo(pitchB2, BASS_CHANNEL),
    ButtonInfo(pitchC3, BASS_CHANNEL),
    ButtonInfo(pitchD3, BASS_CHANNEL),
    ButtonInfo(pitchE3, BASS_CHANNEL),
    ButtonInfo(pitchF3, BASS_CHANNEL),
    ButtonInfo(pitchG2, BASS_CHANNEL),
    ButtonInfo(pitchA2, BASS_CHANNEL),
  }
};

class MidiPressHandler : public IKeyHandler, public IAnalogHandler {
    void onPressed(byte x, byte y) {
      musicKeys[y][x].update(true);
      MidiUSB.flush();
    }

    void onReleased(byte x, byte y) {
      musicKeys[y][x].update(false);
      MidiUSB.flush();
    }

     void onUpdate(byte value) {
      setController(CONTROL_CHANNEL, VOLUME_ID, value);
    }
};

const MidiPressHandler midiHandler;
const FixedArray<byte, rows> rowReadPins(2, 3, 4, 5, 6);
const MatrixKeyboard<columns, rows> keyboard(ShiftRegister(9, 7, 8), rowReadPins);
const MatrixHandler<columns, rows> matrixHandler(keyboard, &midiHandler);
const AnalogReader reader(A0, &midiHandler);

void setup() {
  matrixHandler.onSetup();
  //  setNewKey(DEFAULT_HARMONY_KEY + 4);
}

void loop() {
  matrixHandler.onFrame();
  reader.onFrame();
}
