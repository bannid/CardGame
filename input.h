#ifndef INPUT_H
#define INPUT_H

#include "openglIncludes.h"
namespace Input{
    typedef int(*KeyPressCheckCallback)(GLFWwindow*, int);
    struct Key{
        int keyCode;
        bool keyWasPressed;
        bool keyWasReleased;
        Key();
        Key(int keyCode);
    };
    static Key keyboardKeys[] = {
        Input::Key(GLFW_KEY_LEFT),
        Input::Key(GLFW_KEY_RIGHT),
        Input::Key(GLFW_KEY_UP),
        Input::Key(GLFW_KEY_DOWN)
    };
    
    static Key mouseKeys[] = {
        Input::Key(GLFW_MOUSE_BUTTON_LEFT)
    };
    
    static GLFWwindow * window = NULL;
    void Initialize(GLFWwindow * window);
    //Input functions
    void UpdateInputState();
    void UpdateInputStateInternal(Key * keys, int numberOfKeys,
                                  KeyPressCheckCallback keyPressCheck);
    bool GetKeyFromArray(int keyCode, Key * keys, int numberOfKeys, Key * out);
    bool MouseKeyWasReleased(int keyCode);
    bool KeyboardKeyWasReleased(int keyCode);
    void GetMousePositions(double * x, double * y);
    void ResetState();
}
#endif //INPUT_H
