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

/*matrix calculate*/
double **double_array_malloc(int row, int col)
{
    double** array = (double**)malloc(col*sizeof(double*));
    array[0] = (double*)malloc(row*col*sizeof(double));
    
    int r_index = 0,c_index = 0;
    
    for (c_index = 0;c_index<col;c_index++)
    {
        array[c_index] = (double*)(*array + (row*c_index));
    }
    
    for (c_index = 0; c_index<col; c_index++) {
        for (r_index = 0; r_index<row; r_index++) {
            array[c_index][r_index] = 0;
        }
    }
    
    return array;
}

double* array_malloc(int size)
{
    double *array = (double*)calloc(size*sizeof(double));
    
    return array;
}

/*2D kalman filter for constant varity*/
void KM_2d_init(KM_1d_config* _config,double _R, double _Q, double delta_t)
{
    _config->current_output = array_malloc(2);
    _config->previous_output = array_malloc(2);
    
    _config->F = double_array_malloc(2, 2);
    _config->F[0][0] = _config->F[1][1] = 1;
    _config->F[0][1] = delta_t;
    
    _config->H = array_malloc(2);
    _config->H[0] = 1;
    
    _config->current_estimate = double_array_malloc(2, 2);
    _config->current_estimate[0][1] = _config_->current_estimate[1][0] = 1000;
    
    _config->next_estimate = double_array_malloc(2, 2);
    
    _config->R = _R;
    
    _config->Q = double_array_malloc(2, 2);
    _config->Q[0][1] = _config->Q[1][0] = pow(delta_t,2.0)*0.5*_Q;
    _config->Q[0][0] = pow(delta_t,3.0)*0.3333333333*_Q;
    _config->Q[1][1] = delta_t*_Q;
    
    _config->KM_gain = array_malloc(2);
    
}

double KM_2d_start(KM_1d_config* _config, double measure);
void KM_2d_set_Q(KM_1d_config* _config, double _Q);
void KM_2d_set_R(KM_1d_config* _config, double _R);

