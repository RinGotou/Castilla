#pragma once
#include "castilla.window.h"

namespace castilla {
  /* Experimental implementation. BE CAREFUL */
  enum class ElementVariant{
    Button,
    ImageView,
    ScrollBar_Vertical,
    ScrollBar_Horizontal
  };

  using ProtectedTexture = ProtectedPointer<Texture>;

  class Element {
  protected:
    SDL_Rect src_;
    SDL_Rect dest_;
    int priority_;
    ProtectedTexture texture_;
    bool managed_;

  public:
    int64_t order;

  public:
    ~Element() { 
      if (managed_) {
        SDL_DestroyTexture(texture_->Get());
        texture_.Destroy();
      }
    }

    Element() = delete;

    Element(Texture &texture, SDL_Rect dest) :
      src_{ 0, 0, texture.GetWidth(), texture.GetHeight() },
      dest_(dest), priority_(0), texture_(&texture), order(0),
      managed_(false)
    {}

    Element(Texture &texture, SDL_Rect src, SDL_Rect dest) :
      src_(src), dest_(dest), priority_(0), texture_(&texture),
      order(0), managed_(false)
    {}

    Element(Element &rhs) :
      src_(), dest_(), priority_(0), texture_(rhs.texture_), 
      managed_(false) {
      std::swap(src_, rhs.src_);
      std::swap(dest_, rhs.dest_);
      std::swap(priority_, rhs.priority_);
      std::swap(managed_, rhs.managed_);
    }

    Element(Element &&rhs) :
      src_(), dest_(), priority_(0), texture_(rhs.texture_),
      managed_(false) {
      std::swap(src_, rhs.src_);
      std::swap(dest_, rhs.dest_);
      std::swap(priority_, rhs.priority_);
      std::swap(managed_, rhs.managed_);
    }

    bool Draw(SDL_Renderer *renderer);

    void SetTexture(Texture *texture, bool managed = false) {
      texture_ = texture;
      managed_ = managed;
    }

    SDL_Rect &ExposeSrcRectange() {
      return src_;
    }

    SDL_Rect &ExposeDestRectange() {
      return dest_;
    }

    bool SetPriority(int priority) { 
      if (priority < 0)return false;
      priority_ = priority; 
      return true;
    }

    int GetPriority() const { return priority_; }
  };

  //using ElementLayer = map<string, Element>;
  using NamedElement = pair<const string, Element>;
  using ManagedElement = shared_ptr<Element>;

  class ElementLayer : public map<string, Element> {
  public:
    deque<Element *> drawing_vec;

    NamedElement *FindNamedElementByOrder(int64_t order);
    void ResortVector();
  };

  using ElementMap = map<int, ElementLayer>;

  class PlainWindow : 
    public BasicWindow, 
    public IntrusiveCounter<PlainWindow> {
  protected:
    ElementMap elements_;
    bool real_time_;

    Element *FindElement(string id);

    bool IsElementExist(string id) { 
      return FindElement(id) != nullptr; 
    }
  public:
    virtual ~PlainWindow() {
      auto id = SDL_GetWindowID(window_);
      DisposeWindow(id);
    }

    PlainWindow() = delete;

    PlainWindow(WindowOption option) : BasicWindow(option), real_time_(false) {
      RegisterWindow(this, SDL_GetWindowID(window_));
    }

    virtual void operator=(PlainWindow &rhs) {
      std::swap(window_, rhs.window_);
      std::swap(renderer_, rhs.renderer_);
      std::swap(elements_, rhs.elements_);
      std::swap(real_time_, rhs.real_time_);
    }

    virtual void operator=(PlainWindow &&rhs) {
      this->operator=(rhs);
    }

    void ClearElements();
    bool DrawElements();
    //TODO:Update by events
    //void UpdateElementByEventType

    bool SetElementPosition(string id, SDL_Point point);
    bool SetElementSize(string id, int width, int height);
    bool SetElementDestination(string id, SDL_Rect dest);
    bool SetElementCropper(string id, SDL_Rect cropper);
    bool SetElementOnTop(string id);
    bool SetElementOnBottom(string id);
    bool SetElementTexture(string id, Texture &texture);
    void SetWindowTitle(string title);

    SDL_Point GetElementPosition(string id);
    SDL_Point GetElementSize(string id);
    SDL_Rect GetElementDestination(string id);
    SDL_Rect GetElementCropper(string id);

    bool ElementInRange(string id, SDL_Point point);
    bool AddElement(string id, Element &element);
    NamedElement *FindElementByPoint(SDL_Point point);
    bool DisposeElement(string id);


    bool AddElement(string id, Element &&element) { 
      return AddElement(id, element); 
    }

    Element *GetElementById(string id) { 
      return FindElement(id); 
    }

    void RealTimeRefreshingMode(bool value) {
      real_time_ = value;
    }

    bool GetRefreshingMode() const {
      return real_time_;
    }
  };

  using ManagedPlainWindow = shared_ptr<PlainWindow>;

  void ForceRefreshingAllWindow();
}