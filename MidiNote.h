#ifndef MIDI_NOTE_H
#define MIDI_NOTE_H
#include <MIDIUSB.h>
#include "MidiCommon.h"

class Note : public IPlayable {
  public:
    Note()
      : Note(0)
    {}

    Note(note_t note)
      : note(note)
      , active(false)
      , channel(0)
    {}

    void update(bool isActive, velocity_t velocity = 64) {
      if (isActive && !this->active) {
        this->noteOn(velocity);
      } else if (!isActive && this->active) {
        this->noteOff();
      }
      this->active = isActive;
    }

  private:
    note_t note;
    bool active;
    channel_t channel;

    void noteOn(velocity_t velocity) {
      midiEventPacket_t noteOn = {0x09, 0x90 | this->channel, this->note, velocity};
      MidiUSB.sendMIDI(noteOn);
    }

    void noteOff() {
      midiEventPacket_t noteOff = {0x08, 0x80 | this->channel, this->note, 0};
      MidiUSB.sendMIDI(noteOff);
    }
};

#endif
