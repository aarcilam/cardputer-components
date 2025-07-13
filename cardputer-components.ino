#include <M5Cardputer.h>
#include "Theme.h"
#include "Title.h"
#include "Menu.h"

Menu* menu;

void sayHello() {
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
  M5Cardputer.Display.setCursor(20, 100);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
  M5Cardputer.Display.print("¡Hola Mundo!");
  delay(1500);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
  drawUI();
}

void drawUI() {
  Title title("Menu Principal");
  title.draw();
  menu->draw();
}

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

  menu = new Menu(20, 40, 150);
  menu->addButton("Saludar", sayHello);
  menu->addButton("Otra opción", []() {
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    M5Cardputer.Display.setCursor(20, 100);
    M5Cardputer.Display.print("Elegiste otra opción");
    delay(1500);
    M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);
    drawUI();
  });

  drawUI();
}

void loop() {
  M5Cardputer.update();

  if (M5Cardputer.Keyboard.isChange()) {
    M5Cardputer.Keyboard.updateKeysState();
    auto& word = M5Cardputer.Keyboard.keysState().word;

    for (char key : word) {
      if (key == ';') {
        menu->selectUp();
      }
      if (key == '.') {
        menu->selectDown();
      }
      if (key == '\n' || key == '\r' || key == 'Enter' || key == 'OK' || key == '/') {
        menu->activateSelected();
      }
    }
  }
}

