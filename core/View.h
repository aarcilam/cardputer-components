#pragma once
#include <M5Cardputer.h>
#include "../theme.h"

// Forward declaration
class ViewManager;

class View {
public:
  virtual ~View() = default;
  
  virtual void draw() = 0;
  virtual void handleInput(char key) = 0;
  virtual void onEnter() {}
  virtual void onExit() {}
  
  // Callback para cambiar de vista
  void setViewManager(ViewManager* manager) {
    _viewManager = manager;
  }
  
protected:
  void changeView(View* newView);
  
private:
  ViewManager* _viewManager = nullptr;
}; 