#pragma once
#include "RoutedView.h"
#include "Menu.h"
#include "Title.h"

class MenuView : public RoutedView {
public:
  MenuView() {
    _menu = new Menu(20, 40, 150);
    _menu->addButton("Saludar", sayHelloCallback);
    _menu->addButton("Otra opcion", otherOptionCallback);
    _menu->addButton("Configuracion", settingsCallback);
  }
  
  ~MenuView() {
    delete _menu;
  }
  
  void draw() override {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    Title title("Menu Principal");
    title.draw();
    _menu->draw();
  }
  
  void handleInput(char key) override {
    if (key == ';') {
      _menu->selectUp();
    }
    if (key == '.') {
      _menu->selectDown();
    }
    if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK' || key == '/') {
      _menu->activateSelected();
    }
  }
  
  // Callbacks estáticos
  static void sayHelloCallback() {
    if (_instance) {
      _instance->navigate("/hello");
    }
  }
  
  static void otherOptionCallback() {
    if (_instance) {
      _instance->navigate("/other");
    }
  }
  
  static void settingsCallback() {
    if (_instance) {
      _instance->navigate("/settings");
    }
  }
  
  // Método para establecer la instancia
  static void setInstance(MenuView* instance) {
    _instance = instance;
  }
  
private:
  Menu* _menu;
  static MenuView* _instance;
};

// Definición de la variable estática
MenuView* MenuView::_instance = nullptr; 