#ifndef __GLOBALS_INCLUDED__
#define __GLOBALS_INCLUDED__

//Global functions
void init();
double indexCoincidence(const std::string&);
double evaluate(const std::string&);
void setNGrams();
void loadCipherData();

//Global variables
extern float ENGLISH_IC;
extern float ENGLISH_IC_DIFFERENCE;
extern float TEMP;
extern float STEP;
extern unsigned COUNT;
extern int LENGTH;

#define AMSCO 0
#define BIFID 1
#define BINARY 2
#define CADENUS 3
#define COLUMNAR 4
#define FOURSQUARE 5
#define HILL 6
#define PLAYFAIR 7
#define POLYBIUSSQUARE 8
#define RAILFENCE 9
#define SUBSTITUTION 10
#define TRIFID 11
#define VIGENERE 12

typedef struct
{
    const char* name;
    int nchars;
    int ic;
    int mic;
    int mka;
    int dic;
    int edi;
    int lr;
    int rod;
    int ldi;
    int sdd;
    bool transposition;
} CipherData;

#define MAX_CIPHER 13
extern CipherData CIPHERS[MAX_CIPHER];

extern int LDI_ARR[26][26];
extern int SDD_ARR[26][26];

#endif
