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
    
    KM_1d_update(_config,measure);
    
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

void double_array_free(double** array)
{
    free(array);
}

double* array_malloc(int size)
{
    double *array = (double*)calloc(size*sizeof(double));
    
    return array;
}

void array_free(double* array)
{
    free(array);
}

void array_2d_multiple_1(double** a, double** b, double** out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0];
    out[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1];
    out[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0];
    out[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1];
    
}

void array_2d_multiple_with_transport_1(double** a, double** b, double** out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0][0] = a[0][0]*b[0][0] + a[0][1]*b[0][1];
    out[0][1] = a[0][0]*b[1][0] + a[0][1]*b[1][1];
    out[1][0] = a[1][0]*b[0][0] + a[1][1]*b[0][1];
    out[1][1] = a[1][0]*b[1][0] + a[1][1]*b[1][1];
    
}

void array_2d_multiple_2(double* b, double** a, double* out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0] = a[0][0]*b[0] + a[0][1]*b[1];
    out[1] = a[1][0]*b[0] + a[1][1]*b[1];
    
}

void array_2d_multiple_2(double* a, double* b, double* out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0] = a[0]*b[0] + a[0]*b[1];
    
}

void array_2d_multiple_3(double* a, double* b, double** out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0][0] = a[0]*b[0];
    out[0][1] = a[0]*b[1];
    out[1][0] = a[1]*b[0];
    out[1][1] = a[1]*b[1];
    
}

void array_2d_add(double** a,double** b,double** out)
{
    if ((a == NULL) || (b == NULL) || (out == NULL))
    {
        return;
    }
    
    out[0][0] = a[0][0] + b[0][0];
    out[0][1] = a[0][1] + b[0][1];
    out[1][0] = a[1][0] + b[1][0];
    out[1][1] = a[1][1] + b[1][1];
}


/*2D kalman filter for constant varity*/
void KM_2d_init(KM_2d_config* _config,double _R, double _Q, double delta_t)
{
    _config->current_output = array_malloc(2);
    _config->previous_output = array_malloc(2);
    
    _config->F = double_array_malloc(2, 2);
    _config->F[0][0] = _config->F[1][1] = 1;
    _config->F[0][1] = delta_t;
    
    _config->H = array_malloc(2);
    _config->H[0] = 1;
    
    _config->next_estimate = double_array_malloc(2, 2);
    _config->next_estimate[0][1] = _config_->next_estimate[1][0] = 1000;
    
    _config->current_estimate = double_array_malloc(2, 2);
    
    _config->R = _R;
    
    _config->Q = double_array_malloc(2, 2);
    _config->Q[0][1] = _config->Q[1][0] = pow(delta_t,2.0)*0.5*_Q;
    _config->Q[0][0] = pow(delta_t,3.0)*0.3333333333*_Q;
    _config->Q[1][1] = delta_t*_Q;
    
    _config->KM_gain = array_malloc(2);
    
}

void KM_2d_predict(KM_2d_config* _config)
{
    int r_index = 0, c_index = 0;
    
    for (c_index = 0; c_index < 2 ; c_index++)
    {
        _config->previous_output[c_index] = _config->current_output[0]*_config->F[c_index][0] + _config->current_output[1]*_config->F[c_index][1];

    }

    double** temp_out = double_array_malloc(2, 2);
    
    array_2d_multiple_1(_config->F, _config->next_estimate, temp_out);
    array_2d_multiple_with_transport_1(temp_out, _config->F, _config->current_estimate);
    array_2d_add(_config->current_estimate, _config->Q, _config->current_estimate);
    
    free(temp_out);
    
}

void KM_id_update(KM_2d_config* _config,double measure)
{
    
}

double* KM_2d_start(KM_2d_config* _config, double measure);
{
    KM_2d_predict(_config);
    
    KM_2d_update(_config,measure);
    
    return _config->current_output;
}

void KM_2d_set_Q(KM_2d_config* _config, double _Q, double delta_t)
{
    _config->Q[0][1] = _config->Q[1][0] = pow(delta_t,2.0)*0.5*_Q;
    _config->Q[0][0] = pow(delta_t,3.0)*0.3333333333*_Q;
    _config->Q[1][1] = delta_t*_Q;
}

void KM_2d_set_R(KM_2d_config* _config, double _R)
{
    _config->R = _R;
}

