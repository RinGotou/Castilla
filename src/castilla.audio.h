#include "castilla.common.h"

namespace castilla {
  using Channel = int;
  using MusicType = Mix_MusicType;
  // TODO: decide to using a more powerful audio library?

  class Music : 
    public IntrusiveCounter<Music>,
    public LifeCycleGuard<Music> {
  protected:
    Mix_Music *ptr_;

  public:
    virtual ~Music() {
      //Avoid library panic
      if (IsAudioSubsystemLoaded()) {
        Mix_FreeMusic(ptr_);
      }
    }

    Music() = delete;

    Music(std::string &path) : ptr_(Mix_LoadMUS(path.data())) {}
    Music(std::string &&path) : Music(path) {}
    //Music(SDL_RWops *ops, bool managed) :
    //  ptr_(Mix_LoadMUS_RW(ops, managed ? 1 : 0)) {}

    Music(Music &rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }
    Music(Music &&rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }

    Channel Play(int loop = 0) {
      return Mix_PlayMusic(ptr_, loop);
    }

    MusicType GetMusicType() {
      return Mix_GetMusicType(ptr_);
    }
  };

  class SoundEffect : 
    IntrusiveCounter<SoundEffect>,
    LifeCycleGuard<SoundEffect> {
  protected:
    Mix_Chunk *ptr_;

  public:
    virtual ~SoundEffect() {
      if (IsAudioSubsystemLoaded()) {
        Mix_FreeChunk(ptr_);
      }
    }

    SoundEffect() = delete;

    SoundEffect(std::string &path) : ptr_(Mix_LoadWAV(path.data())) {}
    SoundEffect(std::string &&path) : SoundEffect(path) {}

    SoundEffect(SoundEffect &rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }
    SoundEffect(SoundEffect &&rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }

    Channel Play(Channel channel = -1, int loop = 0, int tick = -1) {
      return Mix_PlayChannelTimed(channel, ptr_, loop, tick);
    }
  };
}