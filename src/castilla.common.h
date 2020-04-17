#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <list>
#include <string>
#include <string_view>
#include <utility>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <optional>
#ifdef _MSC_VER
#include <Windows.h>
#endif

#ifdef _DEBUG
#include <cstdio>
#endif

#include "castilla.utility.h"
#include <toml.hpp>

//for Windows(MSVC) environment
#ifdef _WIN32
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")
#pragma warning(disable:4996)
#ifdef _DEBUG
#pragma comment(lib,"SDL2test.lib")
#endif
#pragma comment(lib,"SDL2_image.lib")
#pragma comment(lib,"SDL2_mixer.lib")
#pragma comment(lib,"SDL2_ttf.lib")
#endif

namespace castilla {
  //TODO: window management storage

  struct AudioInitSpec {
    int frequency;
    uint16_t format;
    int channels;
    int chunksize;
    int flags;
  };

  constexpr AudioInitSpec kDefaultAudioSpec = {
  48000,
  MIX_DEFAULT_FORMAT,
  MIX_DEFAULT_CHANNELS,
  2048,
  MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG
  };

  enum class InitErrorVariant { 
    MainLibraryFailed, 
    MixInitFailed, 
    OpenAudioFailed,
    OK
  };

  InitErrorVariant EnvironmentSetup(const AudioInitSpec &audio = kDefaultAudioSpec);
  void EnvironmentCleanup();
  bool IsAudioSubsystemLoaded();

  std::wstring s2ws(const std::string &s);
  std::string ws2s(const std::wstring &s);
}