#ifndef MIDI_CHORD_H
#define MIDI_CHORD_H

#include "MidiNote.h"
#include "FixedArray.h"

template <byte NOTES_COUNT>
class Chord : public IPlayable {
  public:
    Chord(const FixedArray<note_t, NOTES_COUNT>& notes, channel_t channel)
    {
      for (byte i = 0; i < NOTES_COUNT; i++) {
        this->notes[i] = Note(notes[i], channel);
      }
    }

    void update(bool isActive, velocity_t velocity = 64) {
      for (byte i = 0; i < NOTES_COUNT; i++) {
        this->notes[i].update(isActive, velocity);
      }
    }

  private:
    FixedArray<Note, NOTES_COUNT> notes;
};

template <simple_array_size_t STEPS>
Chord<STEPS> buildFromStructure(note_t pitch, const FixedArray<byte, STEPS>& structure, channel_t channel, byte offset = 0) {
  FixedArray<note_t, STEPS> notes;
  
  for (byte i = 0; i < STEPS; i++) {
    const byte octavesShift = (i + offset) / STEPS; // if overflow -- add octave
    const byte index = (i + offset) % STEPS;
    notes[i] = pitch + structure[index] + octavesShift * 12;
  }
  return Chord<STEPS>(notes, channel);
}

Chord<3> buildMaj(note_t pitch, channel_t channel, byte offset = 0) {
  return buildFromStructure(pitch, FixedArray<byte, 3>(0, 4, 7), channel, offset);
}

Chord<3> buildMin(note_t pitch, channel_t channel, byte offset = 0) {
  return buildFromStructure(pitch, FixedArray<byte, 3>(0, 3, 7), channel, offset);
}

Chord<4> buildMaj7(note_t pitch, channel_t channel, byte offset = 0) {
  return buildFromStructure(pitch, FixedArray<byte, 4>(0, 4, 7, 10), channel, offset);
}

Chord<4> buildMin7(note_t pitch, channel_t channel, byte offset = 0) {
  return buildFromStructure(pitch, FixedArray<byte, 4>(0, 3, 7, 10), channel, offset);
}


#endif
