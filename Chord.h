#ifndef CHORD_H
#define CHORD_H
#include "FixedArray.h"

FixedArray<byte, 3> majStructure(0, 4, 7);
FixedArray<byte, 3> minStructure(0, 3, 7);
FixedArray<byte, 4> maj7Structure(0, 4, 7, 10);
FixedArray<byte, 4> min7Structure(0, 3, 7, 10);

template <simple_array_size_t STEPS>
FixedArray<note_t, STEPS> buildFromStructure(note_t mainTone, const FixedArray<byte, STEPS>& structure, byte offset = 0) {
  FixedArray<note_t, STEPS> notes;

  for (byte i = 0; i < STEPS; i++) {
    const byte octavesShift = (i + offset) / STEPS; // if overflow -- add octave
    const byte index = (i + offset) % STEPS;
    notes[i] = mainTone + structure[index] + octavesShift * 12;
  }

  return notes;
}
#endif
