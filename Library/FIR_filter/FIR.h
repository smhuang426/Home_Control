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
} FIR_CONFIG;

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
void FIR_init(FIR_CONFIG* config, uint8_t filter_type, ...);
double FIR_start_filter(FIR_CONFIG* config, double input);
void FIR_deinit(FIR_CONFIG* config);

#endif
