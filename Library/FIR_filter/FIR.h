#ifndef _FIR_H_
#define _FIR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/*struct define*/
typedef struct FIR_CONFIG{
    uint8_t     filter_type;
    uint8_t     window_type;
    int         order;
    int         fs;
    int         fc1;
    int         fc2;
    double*     coeficient;
    double*     input;
} FIR_CONFI;

typedef enum WINDOW_TYPE{
    RECT   =0x01,
    BARTLETT,
    HANN,
    HAMMING,
    BLACKMAN
} WINDOW_TYPE;

typedef enum FILTER_TYPE{
    LOWPASS   = 0x01,
    HIGHPASS,
    BANDPASS,
    BANDSTOP
} FILTER_TYPE;

/*global function define*/

#endif
