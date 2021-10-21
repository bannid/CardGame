#include "input.h"

namespace Input{
    Key::Key(int keyCode){ 
        this->keyCode = keyCode;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
    
    Key::Key(){
        this->keyCode = 0;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
    
    void Initialize(GLFWwindow * window){
        Input::window = window;
    }
    
    void GetMousePositions(double* x, double * y){
        glfwGetCursorPos(Input::window, x, y);
    }
    
    void UpdateInputState(){
        UpdateInputStateInternal(Input::keyboardKeys, 4, glfwGetKey);
        UpdateInputStateInternal(Input::mouseKeys, 1, glfwGetMouseButton);
        
    }
    
    void UpdateInputStateInternal(Key * keys,
                                  int numberOfKeys,
                                  KeyPressCheckCallback functionToCheckPresses){
        for(int i = 0; i<numberOfKeys; i++){
            Key * currentKey = keys + i;
            int state = functionToCheckPresses(Input::window, currentKey->keyCode);
            if(state == GLFW_PRESS){
                currentKey->keyWasPressed = true;
                currentKey->keyWasReleased = false;
            }
            else if(state == GLFW_RELEASE){
                if(currentKey->keyWasPressed == true){
                    currentKey->keyWasReleased = true;
                }
                else{
                    currentKey->keyWasReleased = false;
                }
                currentKey->keyWasPressed = false;
            }
        }
    }
    
    bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out){
        for(int i = 0; i<numberOfKeys; i++){
            Key key = keys[i];
            if (key.keyCode == keyCode){
                *out = key;
                return true;
            }
        }
        return false;
    }
    
    
    bool MouseKeyWasReleased(int keyCode){
        Key key;
        if(GetKeyFromArray(GLFW_MOUSE_BUTTON_LEFT,
                           Input::mouseKeys,
                           1,
                           &key)){
            return key.keyWasReleased;
        }
        return false;
    }
    
    bool KeyboardKeyWasReleased(int keyCode){
        Key key;
        if(GetKeyFromArray(GLFW_MOUSE_BUTTON_LEFT,
                           Input::keyboardKeys,
                           1,
                           &key)){
            return key.keyWasReleased;
        }
        return false;
    }
}