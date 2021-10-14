#ifndef DEBUG_H
#define DEBUG_H

#if defined(DEBUG)
#define Assert(expression) if(!(expression)) *(int*)0=0
#define DEBUG_OUTPUT(STRING) OutputDebugStringA(STRING);
#elif !defined(DEBUG)
#define Assert(expression)
#define DEBUG_OUTPUT(STRING)
#endif
#endif