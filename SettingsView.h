#pragma once
#include "RoutedView.h"
#include "Title.h"

class SettingsView : public RoutedView {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Title title("Configuracion");
    title.draw();
    
    M5Cardputer.Display.setCursor(20, 80);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("Aqui puedes configurar");
    
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.print("tu Cardputer");
    
    // Mostrar instrucciones para volver
    M5Cardputer.Display.setCursor(20, 140);
    M5Cardputer.Display.print("Presiona Enter para volver");
  }
  
  void handleInput(char key) override {
    if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK' || key == '/') {
      goBack();
    }
  }
}; 