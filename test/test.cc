#include "castilla.ui.h"
#include "castilla.sound.h"

#include <iostream>
#include <vector>

using namespace castilla;
using namespace std;

int main(int argc, char **argv) {
  EnvironmentSetup();
  WindowOption option{
    1366, 768, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    0, "Castilla Test"
  };

  //testing scope
  {
    IntrusivePointer<PlainWindow> window(new PlainWindow(option));
    bool trigger = true;
    SDL_Event event;

    while (trigger) {
      while (SDL_WaitEvent(&event) != 0) {
        if (event.type == SDL_WINDOWEVENT) {

          trigger = !(event.window.event == SDL_WINDOWEVENT_CLOSE);
          break;
        }
      }
    }
  }

  EnvironmentCleanup();
  return 0;
}