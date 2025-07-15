#ifndef INPUT_TEXT_H
#define INPUT_TEXT_H

#include <M5Cardputer.h>
#include "../../theme.h"
#include "../../core/KeyboardService.h"

/**
 * @brief Componente de entrada de texto que usa el KeyboardService
 * 
 * Este componente proporciona una interfaz de entrada de texto con:
 * - Cursor parpadeante
 * - Navegación con teclado
 * - Validación de entrada
 * - Callbacks para confirmar/cancelar
 */
class InputText {
private:
    String _text;
    String _placeholder;
    int _maxLength;
    int _cursorPosition;
    bool _isActive;
    bool _showCursor;
    unsigned long _lastBlinkTime;
    
    // Callbacks
    std::function<void(String)> _onConfirmCallback;
    std::function<void()> _onCancelCallback;
    std::function<void(char)> _onKeyPressCallback;
    
    // Posición en pantalla
    int _x, _y, _width, _height;
    
    // Validación
    std::function<bool(char)> _validator;

public:
    /**
     * @brief Constructor del componente InputText
     * @param x Posición X en pantalla
     * @param y Posición Y en pantalla
     * @param width Ancho del campo
     * @param height Alto del campo
     * @param placeholder Texto de placeholder
     * @param maxLength Longitud máxima del texto
     */
    InputText(int x, int y, int width, int height, const String& placeholder = "", int maxLength = 50) 
        : _x(x), _y(y), _width(width), _height(height), _placeholder(placeholder), _maxLength(maxLength),
          _cursorPosition(0), _isActive(false), _showCursor(true), _lastBlinkTime(0) {
        _text = "";
    }
    
    /**
     * @brief Activar el campo de entrada
     */
    void activate() {
        _isActive = true;
        _cursorPosition = _text.length();
        _showCursor = true;
        _lastBlinkTime = millis();
        
        // Configurar callbacks del KeyboardService
        KeyboardService& keyboard = KeyboardService::getInstance();
        
        keyboard.setOnSelect([this]() {
            if (_onConfirmCallback) {
                _onConfirmCallback(_text);
            }
        });
        
        keyboard.setOnGoBack([this]() {
            if (_onCancelCallback) {
                _onCancelCallback();
            }
        });
        
        keyboard.setOnKeyPressed([this](char key) {
            handleKeyPress(key);
        });
    }
    
    /**
     * @brief Desactivar el campo de entrada
     */
    void deactivate() {
        _isActive = false;
        
        // Limpiar callbacks del KeyboardService
        KeyboardService& keyboard = KeyboardService::getInstance();
        keyboard.clearCallbacks();
    }
    
    /**
     * @brief Dibujar el componente
     */
    void draw() {
        // Fondo del campo
        M5Cardputer.Display.fillRect(_x, _y, _width, _height, Theme::BACKGROUND_COLOR);
        M5Cardputer.Display.drawRect(_x, _y, _width, _height, _isActive ? Theme::ACCENT_COLOR : Theme::TEXT_COLOR);
        
        // Texto o placeholder
        M5Cardputer.Display.setTextColor(Theme::TEXT_COLOR);
        M5Cardputer.Display.setTextSize(1);
        
        String displayText = _text;
        if (displayText.isEmpty() && !_isActive) {
            displayText = _placeholder;
            M5Cardputer.Display.setTextColor(Theme::SECONDARY_TEXT_COLOR);
        }
        
        // Calcular posición del texto
        int textX = _x + 5;
        int textY = _y + (_height / 2) - 4;
        
        // Mostrar texto con scroll si es necesario
        if (displayText.length() > (_width - 10) / 6) { // Aproximadamente 6 píxeles por carácter
            int startPos = max(0, _cursorPosition - (_width - 10) / 6);
            displayText = displayText.substring(startPos);
        }
        
        M5Cardputer.Display.setCursor(textX, textY);
        M5Cardputer.Display.print(displayText);
        
        // Cursor parpadeante si está activo
        if (_isActive) {
            updateCursor();
            
            if (_showCursor) {
                int cursorX = textX + (_cursorPosition * 6); // Aproximadamente 6 píxeles por carácter
                if (cursorX < _x + _width - 5) {
                    M5Cardputer.Display.drawLine(cursorX, textY, cursorX, textY + 8, Theme::ACCENT_COLOR);
                }
            }
        }
    }
    
    /**
     * @brief Establecer callback para cuando se confirma la entrada
     * @param callback Función a ejecutar con el texto ingresado
     */
    void setOnConfirm(std::function<void(String)> callback) {
        _onConfirmCallback = callback;
    }
    
    /**
     * @brief Establecer callback para cuando se cancela la entrada
     * @param callback Función a ejecutar
     */
    void setOnCancel(std::function<void()> callback) {
        _onCancelCallback = callback;
    }
    
    /**
     * @brief Establecer callback para cuando se presiona una tecla
     * @param callback Función a ejecutar con la tecla presionada
     */
    void setOnKeyPress(std::function<void(char)> callback) {
        _onKeyPressCallback = callback;
    }
    
    /**
     * @brief Establecer validador de caracteres
     * @param validator Función que retorna true si el carácter es válido
     */
    void setValidator(std::function<bool(char)> validator) {
        _validator = validator;
    }
    
    /**
     * @brief Obtener el texto actual
     * @return String con el texto
     */
    String getText() const {
        return _text;
    }
    
    /**
     * @brief Establecer el texto
     * @param text Nuevo texto
     */
    void setText(const String& text) {
        _text = text;
        _cursorPosition = _text.length();
    }
    
    /**
     * @brief Limpiar el texto
     */
    void clear() {
        _text = "";
        _cursorPosition = 0;
    }
    
    /**
     * @brief Verificar si está activo
     * @return true si está activo
     */
    bool isActive() const {
        return _isActive;
    }

private:
    void handleKeyPress(char key) {
        if (_onKeyPressCallback) {
            _onKeyPressCallback(key);
        }
        
        // Manejar teclas especiales
        switch (key) {
            case 8: // Backspace
            case 127: // Delete
                if (_cursorPosition > 0) {
                    _text.remove(_cursorPosition - 1, 1);
                    _cursorPosition--;
                }
                break;
                
            case 13: // Enter
                if (_onConfirmCallback) {
                    _onConfirmCallback(_text);
                }
                break;
                
            case 27: // Escape
                if (_onCancelCallback) {
                    _onCancelCallback();
                }
                break;
                
            default:
                // Verificar validador si existe
                if (_validator && !_validator(key)) {
                    return; // Carácter no válido
                }
                
                // Agregar carácter si no excede la longitud máxima
                if (_text.length() < _maxLength) {
                    String newText = _text.substring(0, _cursorPosition) + String(key) + _text.substring(_cursorPosition);
                    _text = newText;
                    _cursorPosition++;
                }
                break;
        }
    }
    
    void updateCursor() {
        unsigned long currentTime = millis();
        if (currentTime - _lastBlinkTime > 500) {
            _showCursor = !_showCursor;
            _lastBlinkTime = currentTime;
        }
    }
};

#endif // INPUT_TEXT_H 