#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "kalman.h"

/*implement function*/
void KM_1d_init(KM_1d_config* _config,double _R, double _Q)
{
    _config->R = _R;
    _config->Q = _Q;
    
    _config->current_output = 0;
    _config->previous_output = 0;
    
    _config->current_estimate = 1000;
    _config->next_estimate = 0;
    
    _config->KM_gain = 0;
}

void KM_1d_predict(KM_1d_config* _config)
{
    _config->previous_output = _config->current_output;
    _config->current_estimate = _config->next_estimate + _config->Q;
}

void KM_id_update(KM_1d_config* _config,double measure)
{
    _config->KM_gain = _config->current_estimate/(_config->current_estimate + _config->R);
    _config->current_output =  _config->previous_output + _config->KM_gain*(measure - _config->previous_output);
    _config->next_estimate = (1 - _config->KM_gain)*_config->current_estimate;
}

double KM_1d_start(KM_1d_config* _config, double measure)
{
    KM_1d_predict(_config);
    
    KM_id_update(_config,measure);
    
    return _config->current_output;
}

void KM_1d_set_Q(KM_1d_config* _config, double _Q)
{
    _config->Q = _Q;
}

void KM_1d_set_R(KM_1d_config* _config, double _R)
{
    _config->R = _R;
}

