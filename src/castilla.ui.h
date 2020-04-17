#include "castilla.common.h"

namespace castilla {
  using Color = SDL_Color;

  class Font : public IntrusiveCounter<Font> {
  protected:
    TTF_Font *ptr_;

  public:
    virtual ~Font() {
      if (TTF_WasInit()) {
        TTF_CloseFont(ptr_);
      }
    }

    Font() = delete;

    Font(std::string path, int size) : ptr_(nullptr) {
      TTF_Init();
      ptr_ = TTF_OpenFont(path.data(), size);
    }
    Font(Font &rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }
    Font(Font &&rhs) : ptr_(nullptr) { std::swap(ptr_, rhs.ptr_); }

    TTF_Font *Get() { return ptr_; }
  };

  enum class ImageType {
    JPG = IMG_INIT_JPG,
    PNG = IMG_INIT_PNG,
    TIF = IMG_INIT_TIF,
    WEBP = IMG_INIT_WEBP
  };

  using Renderer = SDL_Renderer;

  class Texture : 
    public IntrusiveCounter<Texture>,
    public LifeCycleGuard<Texture> {
  protected:
    int width_;
    int height_;
    SDL_Texture *ptr_;

  protected:
    //For rendering multiline text
    int CalculateTextWidth(std::list<std::wstring_view> &text, Font &font);

    int CalculateTextHeight(std::list<std::wstring_view> &text, Font &font) {
      int line_height;
      const auto *dest = (const Uint16 *)(text.front().data());
      TTF_SizeUNICODE(font.Get(), dest, 
        nullptr, &line_height);
      return int(text.size()) * line_height;
    }

    void WrappingSingleUnit(std::wstring_view text, uint32_t wrap_length,
      Font &font, std::list<std::wstring_view> &output);

    void SpiltStringByWrapLength(std::wstring_view text, uint32_t wrap_length,
      Font &font, std::list<std::wstring_view> &output);

  public:
    virtual ~Texture() {
      if (ptr_ != nullptr) {
        SDL_DestroyTexture(ptr_);
      }
    }

    Texture() : width_(0), height_(0), ptr_(nullptr) {}

    //init from image
    Texture(std::string_view image_path, ImageType type, Renderer *renderer,
      std::optional<Color> color = std::nullopt);
    Texture(std::wstring_view text, Font &font, Color color,
      Renderer *renderer, uint32_t wrap_length);


    bool Valid() const { return ptr_ != nullptr; }
  };
}