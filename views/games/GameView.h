#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/title.h"

class GameView : public RoutedView {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Title title("Juegos");
    title.draw();
    
    M5Cardputer.Display.setCursor(20, 80);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("Aqui puedes jugar");
    
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.print("Snake, Tetris, etc.");
    
    // Mostrar instrucciones para volver
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