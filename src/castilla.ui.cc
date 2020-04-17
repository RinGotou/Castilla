#include "castilla.ui.h"

namespace castilla {
  int Texture::CalculateTextWidth(std::list<std::wstring_view> &text, Font &font) {
    int result = -1;
    int temp = -1;
    //choose the longest value as result.
    auto comparing = [&result, &temp](const wchar_t *value, Font &font) -> void {
      TTF_SizeUNICODE(font.Get(), (const uint16_t *)value, &temp, nullptr);
      if (temp > result) result = temp;
    };

    for (auto it = text.begin(); it != text.end(); ++it) {
      comparing(it->data(), font);
    }

    return result;
  }

  Texture::Texture(std::string_view image_path, ImageType type, Renderer *renderer, 
    std::optional<Color> color) : 
    width_(0), height_(0), ptr_(0) {
    //Load image file to surface
    IMG_Init(static_cast<int>(type));
    SDL_Surface *surface = IMG_Load(image_path.data());
    
    //Load failed. return
    if (surface == nullptr) return;

    //color argument is available?
    if (color.has_value()) {
      auto &c = color.value();
      uint32_t color_value = SDL_MapRGBA(surface->format, c.r, c.g, c.b, c.a);
      if (SDL_SetColorKey(surface, 1, color_value) != 0) return;
    }

    //Init texture pointer
    ptr_ = SDL_CreateTextureFromSurface(renderer, surface);
    //Save surface data
    width_ = surface->w;
    height_ = surface->h;

    //Free surface pointer
    SDL_FreeSurface(surface);
  }

  Texture::Texture(std::wstring_view text, Font &font, Color color, 
    Renderer *renderer, uint32_t wrap_length) {
    //wstring wtext = s2ws(text);
    //This function is broken in SDL_TTF 2.0.15
    //surface = TTF_RenderUTF8_Blended_Wrapped(font.Get(), text.data(), color.Get(), wrap_length);

    //Before SDL2 fix this issue, I will try my best.
    //Spilt text string by '\n' and render every line of the original text

    //TODO: wrapping by word/blank characters

    //Main working destination
    SDL_Surface *surface = nullptr;
    //cache for string spilting
    std::list<std::wstring_view> spilted_strings;
    std::wstring_view            temp;

    //Preprocessing for escape characher
    if (wrap_length == 0) {
      //TODO: split string with wstring_view
    }
    else {
      
    }

  }
}