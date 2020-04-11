#include "castilla.common.h"

namespace castilla {
  bool &ExposeAudioSubsystemState() {
    static std::mutex gate;
    std::lock_guard<std::mutex> guard(gate);
    static bool loaded = false;
    return loaded;
  }

  InitErrorVariant EnvironmentSetup(const AudioInitSpec &audio) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      return InitErrorVariant::MainLibraryFailed;
    }

    if (Mix_Init(audio.flags) == 0) {
      return InitErrorVariant::MixInitFailed;
    }

    if (Mix_OpenAudio(
      audio.frequency,
      audio.format,
      audio.channels,
      audio.chunksize) != 0) {
      return InitErrorVariant::OpenAudioFailed;
    }

    ExposeAudioSubsystemState() = true;

    return InitErrorVariant::OK;
  }

  void EnvironmentCleanup() {
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();

    ExposeAudioSubsystemState() = false;

    SDL_Quit();
  }

  bool IsAudioSubsystemLoaded() {
    return ExposeAudioSubsystemState();
  }

  std::wstring s2ws(const std::string &s) {
    if (s.empty()) return std::wstring();
    size_t length = s.size();
    //wchar_t *wc = (wchar_t *)malloc(sizeof(wchar_t) * (length + 2));
    wchar_t *wc = (wchar_t *)calloc(length + 64, sizeof(wchar_t));
    auto res = mbstowcs(wc, s.data(), s.length() + 1);
    std::wstring str(wc);
    free(wc);
    return str;
  }

  std::string ws2s(const std::wstring &s) {
    if (s.empty()) return std::string();
    size_t length = s.size();
    //char *c = (char *)malloc(sizeof(char) * (length + 1) * 2);
    char *c = (char *)calloc((length + 64) * 2, sizeof(char));
    auto res = wcstombs(c, s.data(), (length + 64) * 2);
    std::string result(c);
    free(c);
    return result;
  }
}