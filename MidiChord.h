#ifndef MIDI_CHORD_H
#define MIDI_CHORD_H

#include "MidiNote.h"
#include "SimpleArray.h"

template <byte NOTES_COUNT>
class Chord : public IPlayable {
  public:
    Chord(const SimpleArray<note_t, NOTES_COUNT>& notes)
    {
      for (byte i = 0; i < NOTES_COUNT; i++) {
        this->notes[i] = Note(notes[i]);
      }
    }

    void update(bool isActive, velocity_t velocity = 64) {
      for (byte i = 0; i < NOTES_COUNT; i++) {
        this->notes[i].update(isActive, velocity);
      }
    }

  private:
    SimpleArray<Note, NOTES_COUNT> notes;
};

template <simple_array_size_t STEPS>
Chord<STEPS> buildFromStructure(note_t pitch, const SimpleArray<byte, STEPS>& structure, byte offset = 0) {
  note_t notes[STEPS];
  for (byte i = 0; i < STEPS; i++) {
    const byte octavesShift = (i + offset) / STEPS; // if overflow -- add octave
    const byte index = (i + offset) % STEPS;
    notes[i] = pitch + structure[index] + octavesShift * 12;
  }
  return Chord<STEPS>(SimpleArray<note_t, STEPS>(notes));
}

Chord<3> buildMaj(note_t pitch, byte offset = 0) {
  const byte majStructure[] = {0, 4, 7};
  return buildFromStructure(pitch, SimpleArray<byte, 3>(majStructure), offset);
}

Chord<3> buildMin(note_t pitch, byte offset = 0) {
  const byte minStructure[] = {0, 3, 7};
  return buildFromStructure(pitch, SimpleArray<byte, 3>(minStructure), offset);
}

Chord<4> buildMaj7(note_t pitch, byte offset = 0) {
  const byte maj7Structure[] = {0, 4, 7, 10};
  return buildFromStructure(pitch, SimpleArray<byte, 4>(maj7Structure), offset);
}

Chord<4> buildMin7(note_t pitch, byte offset = 0) {
  const byte min7Structure[] = {0, 3, 7, 10};
  return buildFromStructure(pitch, SimpleArray<byte, 4>(min7Structure), offset);
}


#endif
