#ifndef DEBUG_H
#define DEBUG_H

#if defined(DEBUG)

#define Assert(expression) if(!(expression)) *(int*)0=0
#define DEBUG_OUTPUT(STRING) OutputDebugStringA(STRING);
#define DEBUG_IF(condition) condition

#elif !defined(DEBUG)

#define Assert(expression)
#define DEBUG_OUTPUT(STRING)
#define DEBUG_IF(condition) false //If the debug flag is not set, the compiler should optimize this code as it is always false.
#endif

#endif