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
  
  // Métodos de navegación del teclado
  virtual void onSelect() {
    // Por defecto, no hacer nada
  }
  
  virtual void onGoBack() {
    // Por defecto, no hacer nada
  }
  
  virtual void onNavigateNext() {
    // Por defecto, no hacer nada
  }
  
  virtual void onNavigatePrev() {
    // Por defecto, no hacer nada
  }
  
  // Callback para cambiar de vista
  void setViewManager(ViewManager* manager) {
    _viewManager = manager;
  }
  
  // Métodos para controlar el redibujo
  virtual void clearScreen() {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
  }
  
  virtual bool needsFullRedraw() {
    return true; // Por defecto, redibujar todo
  }
  
  virtual void drawPartial() {
    // Por defecto, llamar al draw completo
    draw();
  }
  
  // Método para refrescar solo el header
  virtual void refreshHeader() {
    // Por defecto, no hacer nada
  }
  
protected:
  void changeView(View* newView);
  
private:
  ViewManager* _viewManager = nullptr;
}; 