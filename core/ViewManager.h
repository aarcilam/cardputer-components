#pragma once
#include "View.h"

class ViewManager {
public:
  ViewManager() : _currentView(nullptr) {}
  
  void setCurrentView(View* view) {
    if (_currentView) {
      _currentView->onExit();
    }
    _currentView = view;
    if (_currentView) {
      _currentView->setViewManager(this);
      _currentView->onEnter();
      _currentView->draw();
    }
  }
  
  void handleInput(char key) {
    if (_currentView) {
      _currentView->handleInput(key);
    }
  }
  
  void draw() {
    if (_currentView) {
      _currentView->draw();
    }
  }
  
  // Método público para cambiar vista
  void changeView(View* newView) {
    setCurrentView(newView);
  }
  
private:
  View* _currentView;
};

// Implementación del método changeView de View
inline void View::changeView(View* newView) {
  if (_viewManager) {
    _viewManager->changeView(newView);
  }
} 