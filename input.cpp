#include "input.h"

void UpdateInputState(GLFWwindow * window,
                      Key * keys,
                      int numberOfKeys,
                      KeyPressCheckCallback functionToCheckPresses){
    for(int i = 0; i<numberOfKeys; i++){
        Key * currentKey = keys + i;
        int state = functionToCheckPresses(window, currentKey->keyCode);
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


bool KeyWasPressed(int keyCode, Key * keysArray, int sizeOfKeysArray){
    Key key;
    if(GetKeyFromArray(GLFW_MOUSE_BUTTON_LEFT,
                       keysArray,
                       sizeOfKeysArray,
                       &key)){
        return key.keyWasReleased;
    }
    return false;
}
