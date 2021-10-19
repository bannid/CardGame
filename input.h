#ifndef INPUT_H
#define INPUT_H

#include "openglIncludes.h"

typedef int(*KeyPressCheckCallback)(GLFWwindow*, int);

struct Key{
    int keyCode;
    bool keyWasPressed;
    bool keyWasReleased;
    Key(int keyCode){ 
        this->keyCode = keyCode;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
    Key(){
        this->keyCode = 0;
        this->keyWasPressed = false;
        this->keyWasReleased = false;
    }
};

//Input functions
void UpdateInputState(GLFWwindow * window, Key * keys, int numberOfKeys,
                      KeyPressCheckCallback keyPressCheck);
bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out);
bool KeyWasPressed(int keyCode, Key * keysArray, int sizeOfKeysArray);

#endif //INPUT_H
