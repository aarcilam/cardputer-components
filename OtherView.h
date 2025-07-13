#pragma once
#include "View.h"

class OtherView : public View {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("Elegiste otra opcion");
    
    // Mostrar instrucciones para volver
    M5Cardputer.Display.setCursor(20, 140);
    M5Cardputer.Display.print("Presiona Enter para volver");
  }
  
  void handleInput(char key) override {
    if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK' || key == '/') {
      changeView(_menuView);
    }
  }
  
  void setMenuView(View* menuView) {
    _menuView = menuView;
  }
  
private:
  View* _menuView = nullptr;
}; 