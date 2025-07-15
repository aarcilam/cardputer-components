#pragma once
#include "../../core/RoutedView.h"

class SplashView : public RoutedView {
private:
  unsigned long startTime;
  bool initialized = false;

public:
  void onEnter() override {
    startTime = millis();
    initialized = true;
  }

  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, 30);
    M5Cardputer.Display.print("Iniciando...");

    // Logo o título principal
    M5Cardputer.Display.setTextSize(3);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.setCursor(10, 60);
    M5Cardputer.Display.print("M5CARDPUTER");
    M5Cardputer.Display.setCursor(10, 90);
    M5Cardputer.Display.print("COMPONENTS");
    M5Cardputer.Display.setCursor(10, 120);;
    
    // Subtítulo
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setCursor(10, 120);
    M5Cardputer.Display.print("by @aarcilam");
    
    // Verificar si han pasado 2 segundos
    if (initialized && (millis() - startTime) >= 2000) {
      navigate("/");
    }
  }
  
  void handleInput(char key) override {
    // No necesitamos manejar input específico aquí
  }
  
  void onSelect() override {
    // Permitir saltar la pantalla de splash con cualquier tecla
    navigate("/");
  }
}; 