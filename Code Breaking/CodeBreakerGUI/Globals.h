#ifndef __GLOBALS_INCLUDED__
#define __GLOBALS_INCLUDED__
#include "Cipher.h"
#include "CodeBreakerMain.h"

//Global functions
double indexCoincidence(const std::string&);
double evaluate(const std::string&);

//Global variables
extern float ENGLISH_IC;
extern float ENGLISH_IC_DIFFERENCE;
extern float TEMP;
extern float STEP;
extern unsigned COUNT;
extern int LENGTH;
extern unsigned MAX_RAILS;

#endif
