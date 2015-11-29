#include "decrypt.h"

#include "Globals.h"
#include "Amsco.h"
#include "Bifid.h"
#include "BinaryConvert.h"
#include "Cadenus.h"
#include "ColumnarTransposition.h"
#include "FourSquare.h"
#include "Hill.h"
#include "Playfair.h"
#include "PolybiusSquare.h"
#include "RailFence.h"
#include "SimpleSubstitution.h"
#include "Trifid.h"
#include "Vigenere.h"

DecryptionWorker* pWorker = NULL;

void DecryptionWorker::decryptCipher()
{
    switch(cipher)
    {
    case AMSCO:
        amsco(text);
        break;
    case BIFID:
        bifid(text);
        break;
    case BINARY:
        binary(text);
        break;
    case CADENUS:
        cadenus(text);
        break;
    case COLUMNAR:
        columnarTransposition(text);
        break;
    case FOURSQUARE:
        fourSquare(text);
        break;
    case HILL:
        hill(text);
        break;
    case PLAYFAIR:
        playfair(text);
        break;
    case POLYBIUSSQUARE:
        polybiusSquare(text);
        break;
    case RAILFENCE:
        railFence(text);
        break;
    case SUBSTITUTION:
        simpleSubstitution(text);
        break;
    case TRIFID:
        trifid(text);
        break;
    case VIGENERE:
        vigenere(text);
        break;
    }
}
