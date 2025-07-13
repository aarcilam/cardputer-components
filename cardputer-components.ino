#include <M5Cardputer.h>
#include "Theme.h"
#include "Router.h"
#include "MenuView.h"
#include "HelloView.h"
#include "OtherView.h"
#include "SettingsView.h"

Router router;
MenuView* menuView;
HelloView* helloView;
OtherView* otherView;
SettingsView* settingsView;

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

  // Crear las vistas
  menuView = new MenuView();
  helloView = new HelloView();
  otherView = new OtherView();
  settingsView = new SettingsView();
  
  // Configurar la instancia estática de MenuView
  MenuView::setInstance(menuView);
  
  // Configurar el router con las rutas
  router.addRoute("/", menuView);
  router.addRoute("/hello", helloView);
  router.addRoute("/other", otherView);
  router.addRoute("/settings", settingsView);
  
  // Configurar las referencias al router en las vistas
  menuView->setRouter(&router);
  helloView->setRouter(&router);
  otherView->setRouter(&router);
  settingsView->setRouter(&router);
  
  // Iniciar con la vista del menú
  router.setInitialRoute("/");
}

void loop() {
  M5Cardputer.update();

  if (M5Cardputer.Keyboard.isChange()) {
    M5Cardputer.Keyboard.updateKeysState();
    auto& word = M5Cardputer.Keyboard.keysState().word;

    for (char key : word) {
      router.handleInput(key);
    }
  }
}

