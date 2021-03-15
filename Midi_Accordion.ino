#include <LiquidCrystal.h>

#include "Common.h"
#include "MatrixHandler.h"
#include "AnalogHandler.h"
#include "Chord.h"
#include "EncoderHandler.h"
#include <PitchToNote.h>

const byte NOTE_COUNT = 12;

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

const byte VOLUME_ID = 7;
const byte ALL_NOTES_OFF = 123;


// PIN MAPPING

const byte PIN_ROW_1 = 2;
const byte PIN_ROW_2 = 3;
const byte PIN_ROW_3 = 4;
const byte PIN_ROW_4 = 5;
const byte PIN_ROW_5 = 6;
const byte PIN_COL_SHCP = 9;
const byte PIN_COL_DS = 7;
const byte PIN_COL_STCP = 8;
const byte PIN_PEDAL_VALUE = A0;
const byte PIN_PEDAL_CONNECTED = 16;
const byte PIN_ENCODER_1 = 0;
const byte PIN_ENCODER_2 = 1;
const byte PIN_LCD_RS = A2;
const byte PIN_LCD_EN = A3;
const byte PIN_LCD_D4 = A1;
const byte PIN_LCD_D5 = 10;
const byte PIN_LCD_D6 = 15;
const byte PIN_LCD_D7 = 14;



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

void setController(channel_t channel, byte id, byte val) {
  midiEventPacket_t setVolume = {0x0B, 0xB0 | channel, id, val};
  MidiUSB.sendMIDI(setVolume);
}

void allNotesOff(channel_t channel) {
  setController(channel, ALL_NOTES_OFF, 0);
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
      ButtonOctave,
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
        case ButtonOctave:
          this->count = 2;
          notes[0] = _tone;
          notes[1] = _tone - 12;
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
    ButtonInfo(pitchE5b, MELODY_CHANNEL, ButtonInfo::ButtonOctave),
    ButtonInfo(pitchA4b, MELODY_CHANNEL, ButtonInfo::ButtonOctave),
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
    ButtonInfo(pitchG5b, MELODY_CHANNEL, ButtonInfo::ButtonOctave),
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
const Button pedalInUse(PIN_PEDAL_CONNECTED, true);
const LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

char data[16];

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
      value /= 2; // 0 - 100%
      sprintf(data, "V %d  ", value);
      lcd.setCursor(0, 1);
      lcd.print(data);
      setController(CONTROL_CHANNEL, VOLUME_ID, value);
      MidiUSB.flush();
    }


};

class ScrollLcdHandler : public IEncoderHandler {
  public:
    ScrollLcdHandler() {
      this->updateLcd();
    }

    void onSetup() {
      this->updateLcd();

    }
    void onIncrease() {
      setNewKey(TARGET_KEY + 1);
      allNotesOff(MELODY_CHANNEL);
      allNotesOff(BASS_CHANNEL);
      allNotesOff(CHORD_CHANNEL);
      MidiUSB.flush();
      this->updateLcd();
    }
    
    void onDecrease() {
      setNewKey(TARGET_KEY - 1);
      allNotesOff(MELODY_CHANNEL);
      allNotesOff(BASS_CHANNEL);
      allNotesOff(CHORD_CHANNEL);
      MidiUSB.flush();
      this->updateLcd();
    }

    void updateLcd() {
      sprintf(data, "%c%c%d", this->notes[this->getPitchTone()], this->sharps[this->getPitchTone()], this->getPitchOctave());
      lcd.setCursor(0, 0);
      lcd.print(data);
    }

    byte getPitchTone() {
      return (TARGET_KEY - pitchA0) % NOTE_COUNT;
    }

    byte getPitchOctave() {
      return (TARGET_KEY - pitchB0 + NOTE_COUNT - 1) / NOTE_COUNT; // math.ceil
    }

  private:
    char notes[12] = "AABCCDDEFFGG";
    char sharps[12] = " #  # #  # #";
};

const MidiPressHandler midiHandler;
const FixedArray<byte, rows> rowReadPins(PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4, PIN_ROW_5);
const MatrixKeyboard<columns, rows> keyboard(ShiftRegister(PIN_COL_SHCP, PIN_COL_DS, PIN_COL_STCP), rowReadPins);
const MatrixHandler<columns, rows> matrixHandler(keyboard, &midiHandler);
const AnalogReader pedalValueReader(PIN_PEDAL_VALUE, &midiHandler);


const ScrollLcdHandler encHandler;
const EncoderReader encReader(PIN_ENCODER_1, PIN_ENCODER_2, &encHandler);

void setup() {
  lcd.begin(8, 2);

  matrixHandler.onSetup();
  encReader.onSetup();
  pedalInUse.onSetup(INPUT_PULLUP);
}

void loop() {
  matrixHandler.onFrame();
  encReader.onFrame();
  pedalInUse.onFrame();
  if (pedalInUse.isPressed()) {
    pedalValueReader.onFrame();
  } else if (pedalInUse.justReleased()) {
    lcd.setCursor(0, 1);
    lcd.print("        ");
    setController(CONTROL_CHANNEL, VOLUME_ID, 127);
    MidiUSB.flush();
  }
}
