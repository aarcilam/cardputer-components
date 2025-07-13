#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/title.h"

class ToolsView : public RoutedView {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Title title("Herramientas");
    title.draw();
    
    M5Cardputer.Display.setCursor(20, 80);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("Calculadora, Reloj,");
    
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.print("Notas, etc.");
    
    // Mostrar instrucciones para volver
    M5Cardputer.Display.setCursor(20, 140);
    M5Cardputer.Display.print("Presiona Enter para volver");
  }
  
  void handleInput(char key) override {
    if (key == ',' ) {
      goBack();
    }
  }
}; 