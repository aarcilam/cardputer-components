#pragma once
#include "../../core/RoutedView.h"

class HelloView : public RoutedView {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("¡Hola Mundo!");
    
    // Mostrar instrucciones para volver
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setCursor(20, 140);
    M5Cardputer.Display.print("Enter: Volver | Del: Volver");
  }

  void onSelect() override {
    goBack();
  }
  
  void handleInput(char key) override {
    // No necesitamos manejar input específico aquí
  }
  
  void onGoBack() override {
    goBack();
  }
}; 