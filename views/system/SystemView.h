#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/title.h"
#include "../../core/NetworkService.h"

class SystemView : public RoutedView {
public:
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    
    Title title("Sistema");
    title.draw();
    
    M5Cardputer.Display.setCursor(20, 80);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
    M5Cardputer.Display.print("Informacion del sistema");
    
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.print("Bateria, Memoria, etc.");
    
    // Información de red
    NetworkService& network = NetworkService::getInstance();
    M5Cardputer.Display.setCursor(20, 120);
    M5Cardputer.Display.print("Red: ");
    M5Cardputer.Display.print(network.isConnected() ? "Conectado" : "Desconectado");
    
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