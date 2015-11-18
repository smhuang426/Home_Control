#ifndef _FIR_H_
#define _FIR_H_

#include <stdint.h>
#include <stdbool.h>

/*struct define*/
typedef struct KM_1d_config{
    
    double current_output;
    double previous_output;
    double current_estimate;
    double next_estimate;
    double R;
    double Q;
    double KM_gain;
    
} KM_1d_config;


/*global function define*/
void KM_1d_init(KM_1d_config* _config,double _R, double _Q);
double KM_1d_start(KM_1d_config* _config, double measure);
void KM_1d_set_Q(KM_1d_config* _config, double _Q);
void KM_1d_set_R(KM_1d_config* _config, double _R);

#endif
