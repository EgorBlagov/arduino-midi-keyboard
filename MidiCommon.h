#ifndef MIDI_COMMON_H
#define MIDI_COMMON_H

typedef byte note_t;
typedef byte velocity_t;
typedef byte channel_t;

class IPlayable {
  public:
    virtual void update(bool isActive, velocity_t velocity = 64) = 0;
};

#endif
