#ifndef INPUT_H
#define INPUT_H

#include "openglIncludes.h"
#define INPUT_MOUSE_KEY_DOWN(name) bool name(int keyCode)
#define INPUT_GET_MOUSE_POSITION(name) void name(double * x, double * y)
#define INPUT_RESET(name) void name(void)

typedef INPUT_MOUSE_KEY_DOWN(MouseKeyIsDownCallback);
typedef INPUT_GET_MOUSE_POSITION(GetMousePositionCallback);
typedef INPUT_RESET(ResetInputCallback);

namespace Input{
    typedef int(*KeyPressCheckCallback)(GLFWwindow*, int);
    struct Key{
        int keyCode;
        bool keyWasPressed;
        bool keyWasReleased;
        Key();
        Key(int keyCode);
    };
    struct InputObject {
        Key * mouseKeys = NULL;
        Key * keyboardKeys = NULL;
        int numberOfMouseKeys = 0;
        int numberOfKeyboardKeys = 0;
    };
    static Key * keyboardKeys;
    static Key * mouseKeys;
    
    static GLFWwindow * window = NULL;
    void Initialize(GLFWwindow * window, Key * keyboardKeys, Key * mouseKeys);
    //Input functions
    void UpdateInputState();
    bool MouseKeyWasReleased(int keyCode);
    bool KeyboardKeyWasReleased(int keyCode);
    void GetMousePositions(double * x, double * y);
    void ResetState();
}
#endif //INPUT_H
