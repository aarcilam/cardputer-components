#ifndef KEYBOARD_SERVICE_H
#define KEYBOARD_SERVICE_H

#include <M5Cardputer.h>
#include <functional>

/**
 * @brief Servicio centralizado para manejar las pulsaciones de teclas del M5Cardputer
 * 
 * Este servicio proporciona una interfaz unificada para manejar las pulsaciones
 * de teclas y define acciones comunes como select, goback, navigate next y prev.
 * Se puede usar de forma independiente sin afectar el sistema de rutas existente.
 */
class KeyboardService {
private:
    static KeyboardService* instance;
    
    // Callbacks para las acciones
    std::function<void()> onSelectCallback;
    std::function<void()> onGoBackCallback;
    std::function<void()> onNavigateNextCallback;
    std::function<void()> onNavigatePrevCallback;
    std::function<void(char)> onKeyPressedCallback;
    
    // Estado del teclado para evitar múltiples activaciones
    bool lastEnterState;
    bool lastDelState;
    bool lastUpState;
    bool lastDownState;
    bool lastLeftState;
    bool lastRightState;
    bool lastEscapeState;
    
    KeyboardService() : 
        lastEnterState(false), 
        lastDelState(false), 
        lastUpState(false), 
        lastDownState(false), 
        lastLeftState(false), 
        lastRightState(false),
        lastEscapeState(false) {}

public:
    /**
     * @brief Obtiene la instancia singleton del servicio
     * @return Referencia al servicio de teclado
     */
    static KeyboardService& getInstance() {
        if (instance == nullptr) {
            instance = new KeyboardService();
        }
        return *instance;
    }
    
    /**
     * @brief Inicializa el servicio de teclado
     */
    void begin() {
        // No se necesita inicialización específica del teclado
        // M5Cardputer ya se inicializa en el setup principal
    }
    
    /**
     * @brief Actualiza el estado del teclado y procesa las pulsaciones
     * Debe ser llamado en cada iteración del loop principal
     */
    void update() {
        // Actualizar el estado del teclado
        M5Cardputer.update();
        
        if (M5Cardputer.Keyboard.isChange()) {
            M5Cardputer.Keyboard.updateKeysState();
            auto& keysState = M5Cardputer.Keyboard.keysState();
            
            // Procesar teclas de texto
            for (char key : keysState.word) {
                if (onKeyPressedCallback) {
                    onKeyPressedCallback(key);
                }
            }
            
            // Procesar Enter (Select)
            if (keysState.enter && !lastEnterState) {
                if (onSelectCallback) {
                    onSelectCallback();
                }
            }
            lastEnterState = keysState.enter;

            // Procesar Del (GoBack)
            if (keysState.del && !lastDelState) {
                if (onGoBackCallback) {
                    onGoBackCallback();
                }
            }
            lastDelState = keysState.del;
            
            // Procesar teclas de navegación basándose en caracteres
            for (char key : keysState.word) {
                // Procesar Up (tecla ';')
                if (key == ';' && !lastUpState) {
                    if (onNavigatePrevCallback) {
                        onNavigatePrevCallback();
                    }
                    lastUpState = true;
                }
                
                // Procesar Down (tecla '.')
                if (key == '.' && !lastDownState) {
                    if (onNavigateNextCallback) {
                        onNavigateNextCallback();
                    }
                    lastDownState = true;
                }
            }
            
            // Resetear estados si no hay teclas presionadas
            if (keysState.word.empty()) {
                lastUpState = false;
                lastDownState = false;
            }
        }
    }
    
    /**
     * @brief Establece el callback para la acción select (Enter)
     * @param callback Función a ejecutar cuando se presiona Enter
     */
    void setOnSelect(std::function<void()> callback) {
        onSelectCallback = callback;
    }
    
    /**
     * @brief Establece el callback para la acción goback (Del/Backspace)
     * @param callback Función a ejecutar cuando se presiona Del
     */
    void setOnGoBack(std::function<void()> callback) {
        onGoBackCallback = callback;
    }
    
    /**
     * @brief Establece el callback para navegar al siguiente elemento (Down/Right)
     * @param callback Función a ejecutar cuando se presiona Down o Right
     */
    void setOnNavigateNext(std::function<void()> callback) {
        onNavigateNextCallback = callback;
    }
    
    /**
     * @brief Establece el callback para navegar al elemento anterior (Up/Left)
     * @param callback Función a ejecutar cuando se presiona Up o Left
     */
    void setOnNavigatePrev(std::function<void()> callback) {
        onNavigatePrevCallback = callback;
    }
    
    /**
     * @brief Establece el callback para cuando se presiona cualquier tecla
     * @param callback Función a ejecutar con el carácter presionado
     */
    void setOnKeyPressed(std::function<void(char)> callback) {
        onKeyPressedCallback = callback;
    }
    
    /**
     * @brief Limpia todos los callbacks
     */
    void clearCallbacks() {
        onSelectCallback = nullptr;
        onGoBackCallback = nullptr;
        onNavigateNextCallback = nullptr;
        onNavigatePrevCallback = nullptr;
        onKeyPressedCallback = nullptr;
    }
    
    /**
     * @brief Verifica si se presionó Enter
     * @return true si Enter fue presionado
     */
    bool isEnterPressed() const {
        return M5Cardputer.Keyboard.keysState().enter;
    }
    
    /**
     * @brief Verifica si se presionó Del/Backspace
     * @return true si Del fue presionado
     */
    bool isDelPressed() const {
        return M5Cardputer.Keyboard.keysState().del;
    }
    
    /**
     * @brief Verifica si se presionó Up (tecla ';')
     * @return true si Up fue presionado
     */
    bool isUpPressed() const {
        // Verificar si la tecla ';' está en las teclas presionadas
        auto& keysState = M5Cardputer.Keyboard.keysState();
        for (char key : keysState.word) {
            if (key == ';') return true;
        }
        return false;
    }
    
    /**
     * @brief Verifica si se presionó Down (tecla '.')
     * @return true si Down fue presionado
     */
    bool isDownPressed() const {
        // Verificar si la tecla '.' está en las teclas presionadas
        auto& keysState = M5Cardputer.Keyboard.keysState();
        for (char key : keysState.word) {
            if (key == '.') return true;
        }
        return false;
    }
    
    /**
     * @brief Verifica si se presionó Left
     * @return true si Left fue presionado
     */
    bool isLeftPressed() const {
        // Por ahora, no hay tecla específica para left
        return false;
    }
    
    /**
     * @brief Verifica si se presionó Right
     * @return true si Right fue presionado
     */
    bool isRightPressed() const {
        // Por ahora, no hay tecla específica para right
        return false;
    }
    
    /**
     * @brief Obtiene el estado actual del teclado
     * @return Estado del teclado
     */
    Keyboard_Class::KeysState getKeysState() const {
        return M5Cardputer.Keyboard.keysState();
    }
    
    /**
     * @brief Verifica si hubo cambios en el teclado
     * @return true si hubo cambios
     */
    bool isChange() const {
        return M5Cardputer.Keyboard.isChange();
    }
    
    /**
     * @brief Obtiene las teclas de texto presionadas
     * @return String con las teclas presionadas
     */
    String getPressedKeys() const {
        String result = "";
        auto& keysState = M5Cardputer.Keyboard.keysState();
        for (char key : keysState.word) {
            result += key;
        }
        return result;
    }
};

// Inicializar la instancia estática
KeyboardService* KeyboardService::instance = nullptr;

#endif // KEYBOARD_SERVICE_H 