#pragma once
#include "View.h"
#include "Menu.h"
#include "Title.h"

class MenuView : public View {
public:
  MenuView() {
    _menu = new Menu(20, 40, 150);
    _menu->addButton("Saludar", sayHelloCallback);
    _menu->addButton("Otra opción", otherOptionCallback);
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
  
  void setViews(View* helloView, View* otherView) {
    _helloView = helloView;
    _otherView = otherView;
  }
  
  // Callbacks estáticos
  static void sayHelloCallback() {
    if (_instance && _instance->_helloView) {
      _instance->changeView(_instance->_helloView);
    }
  }
  
  static void otherOptionCallback() {
    if (_instance && _instance->_otherView) {
      _instance->changeView(_instance->_otherView);
    }
  }
  
  // Método para establecer la instancia
  static void setInstance(MenuView* instance) {
    _instance = instance;
  }
  
private:
  Menu* _menu;
  View* _helloView = nullptr;
  View* _otherView = nullptr;
  static MenuView* _instance;
};

// Definición de la variable estática
MenuView* MenuView::_instance = nullptr; 