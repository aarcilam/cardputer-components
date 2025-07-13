#pragma once
#include "../../core/RoutedView.h"
#include "../../components/ui/ScrollableMenu.h"
#include "../../components/ui/title.h"

class MenuView : public RoutedView {
public:
  MenuView() {
    _menu = new ScrollableMenu(20, 40, 150, 4); // 4 opciones visibles a la vez
    _menu->addButton("Saludar", sayHelloCallback);
    _menu->addButton("Otra opcion", otherOptionCallback);
    _menu->addButton("Configuracion", settingsCallback);
    _menu->addButton("Juegos", gamesCallback);
    _menu->addButton("Herramientas", toolsCallback);
    _menu->addButton("Sistema", systemCallback);
    _menu->addButton("Acerca de", aboutCallback);
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
  
  static void gamesCallback() {
    if (_instance) {
      _instance->navigate("/games");
    }
  }
  
  static void toolsCallback() {
    if (_instance) {
      _instance->navigate("/tools");
    }
  }
  
  static void systemCallback() {
    if (_instance) {
      _instance->navigate("/system");
    }
  }
  
  static void aboutCallback() {
    if (_instance) {
      _instance->navigate("/about");
    }
  }
  
  // Método para establecer la instancia
  static void setInstance(MenuView* instance) {
    _instance = instance;
  }
  
private:
  ScrollableMenu* _menu;
  static MenuView* _instance;
};

// Definición de la variable estática
MenuView* MenuView::_instance = nullptr; 