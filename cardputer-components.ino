#include <M5Cardputer.h>
#include "theme.h"
#include "core/Router.h"
#include "views/main/MenuView.h"
#include "views/main/SplashView.h"
#include "views/main/HelloView.h"
#include "views/main/OtherView.h"
#include "views/system/SettingsView.h"
#include "views/games/GameView.h"
#include "views/tools/ToolsView.h"
#include "views/tools/NetworkView.h"
#include "views/system/SystemView.h"
#include "views/about/AboutView.h"

Router router;
SplashView* splashView;
MenuView* menuView;
HelloView* helloView;
OtherView* otherView;
SettingsView* settingsView;
GameView* gameView;
ToolsView* toolsView;
NetworkView* networkView;
SystemView* systemView;
AboutView* aboutView;

void setup() {
  M5Cardputer.begin();
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(Theme::FONT_NORMAL);
  M5Cardputer.Display.fillScreen(Theme::BACKGROUND_COLOR);

  // Crear las vistas
  splashView = new SplashView();
  menuView = new MenuView();
  helloView = new HelloView();
  otherView = new OtherView();
  settingsView = new SettingsView();
  gameView = new GameView();
  toolsView = new ToolsView();
  networkView = new NetworkView();
  systemView = new SystemView();
  aboutView = new AboutView();
  
  // Configurar la instancia estática de MenuView
  MenuView::setInstance(menuView);
  
  // Configurar el router con las rutas
  router.addRoute("/splash", splashView);
  router.addRoute("/", menuView);
  router.addRoute("/hello", helloView);
  router.addRoute("/other", otherView);
  router.addRoute("/settings", settingsView);
  router.addRoute("/games", gameView);
  router.addRoute("/tools", toolsView);
  router.addRoute("/network", networkView);
  router.addRoute("/system", systemView);
  router.addRoute("/about", aboutView);
  
  // Configurar las referencias al router en las vistas
  splashView->setRouter(&router);
  menuView->setRouter(&router);
  helloView->setRouter(&router);
  otherView->setRouter(&router);
  settingsView->setRouter(&router);
  gameView->setRouter(&router);
  toolsView->setRouter(&router);
  networkView->setRouter(&router);
  systemView->setRouter(&router);
  aboutView->setRouter(&router);
  
  // Iniciar con la vista de splash
  router.setInitialRoute("/splash");
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

