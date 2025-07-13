#include <M5Cardputer.h>
#include "Theme.h"
#include "ViewManager.h"
#include "MenuView.h"
#include "HelloView.h"
#include "OtherView.h"

ViewManager viewManager;
MenuView* menuView;
HelloView* helloView;
OtherView* otherView;

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

  // Crear las vistas
  menuView = new MenuView();
  helloView = new HelloView();
  otherView = new OtherView();
  
  // Configurar la instancia estática de MenuView
  MenuView::setInstance(menuView);
  
  // Configurar las referencias entre vistas
  menuView->setViews(helloView, otherView);
  helloView->setMenuView(menuView);
  otherView->setMenuView(menuView);
  
  // Iniciar con la vista del menú
  viewManager.setCurrentView(menuView);
}

void loop() {
  M5Cardputer.update();

  if (M5Cardputer.Keyboard.isChange()) {
    M5Cardputer.Keyboard.updateKeysState();
    auto& word = M5Cardputer.Keyboard.keysState().word;

    for (char key : word) {
      viewManager.handleInput(key);
    }
  }
}

