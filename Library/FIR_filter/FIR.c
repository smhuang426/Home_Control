#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FIR.h"

#define PI        3.14159265358979323846

/*implement function*/

/*4 type of filter as below*/
double h_low_pass(int _fs,int _fc, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc = (double)_fc;
    double M = (order*0.5);
    double Wc = 2*PI*fc/fs;
    
    double ret = 0.0;
    
    if (M == n)
    {
        ret = Wc/PI;
    }else{
        ret = sin(Wc*(n-M))/(PI * n - M);
    }
    return ret;
}

double h_high_pass(int _fs,int _fc, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc = (double)_fc;
    double M = (order*0.5);
    double Wc = 2*PI*fc/fs;
    
    double ret = 0.0;
    
    if (M == n)
    {
        ret = -1*sin(Wc*(n-M))/(PI * n - M);
    }else{
        ret = 1 - (Wc/PI);
    }
    return ret;
}

double h_band_pass(int _fs,int _fc1, int _fc2, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc1 = (double)_fc1,fc2 = (double)_fc2;
    double M = (order*0.5);
    double Wc1 = 2*PI*fc1/fs , Wc2 = 2*PI*fc2/fs;
    
    double ret = 0.0;
    
    if (M == n)
    {
        ret = ((Wc2 - Wc1)/PI) ;
    }else{
        ret = (sin(Wc2*(n-M))/(PI * n - M)) - (sin(Wc1*(n-M))/(PI * n - M));
    }
    
    return ret;
}

double h_band_stop(int _fs,int _fc1, int _fc2, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc1 = (double)_fc1,fc2 = (double)_fc2;
    double M = (order*0.5);
    double Wc1 = 2*PI*fc1/fs , Wc2 = 2*PI*fc2/fs;
    
    double ret = 0.0;
    
    if (M == n)
    {
        ret = 1 - ((Wc2 - Wc1)/PI) ;
    }else{
        ret = (sin(Wc1*(n-M))/(PI * n - M)) - (sin(Wc2*(n-M))/(PI * n - M));
    }
    
    return ret;
}

double FIR_filter_function(uint8_t type, FIR_CONFIG* config, int _n)
{
    double ret = 0.0;
    
    switch (type) {
        case LOWPASS:
            ret = h_low_pass(config->fs, config->fc1, config->order, _n);
            break;
            
        case HIGHPASS:
            ret = h_high_pass(config->fs, config->fc1, config->order, _n);
            break;
            
        case BANDPASS:
            ret = h_band_pass(config->fs, config->fc1, config->fc2, config->order, _n);
            break;
            
        case BANDSTOP:
            ret = h_band_stop(config->fs, config->fc1, config->fc2, config->order, _n);
            break;
        default:
            ret = 0.0;
            break;
    }
    
    return ret;
}

/*window function as below*/

double Bartlett_window(int _order, int _n)
{
    double order = (double)_order, n = (double)_n;
    
    double ret = 0.0;
    if ((n>=0) && (n <= ((order-1)*0.5) ))
    {
        ret = (2*n)/(order - 1);
    }else if (( n>=((order+1)*0.5) ) && ( n<= (order - 1)))
    {
        ret = 2 - (2*n)/(order - 1);
    }else{
        return 0;
    }
    
    return ret;
}

double cosine_window(double A_para, double B_para,double C_para, int _order, int _n)
{
    double order = (double)_order, n = (double)_n;
 
    if ((n<0) || (n>(order-1)))
    {
        return 0;
    }
    
    double ret = 0.0;
    
    ret = A_para - B_para*cos((2*PI*n)/(order-1)) + C_para*cos((4*PI*n)/(order-1));

    return ret;
}

double FIR_window_function(uint8_t type, int _order, int _n)
{
    double ret = 0.0;
    
    switch (type) {
        case RECT:
            ret = cosine_window(1.0 , 0.0, 0.0 , _order, _n);
            break;
            
        case BARTLETT:
            ret = Bartlett_window(_order, _n);
            break;
            
        case HANN:
            ret = cosine_window(0.5 , 0.5, 0.0 , _order, _n);
            break;
            
        case HAMMING:
            ret = cosine_window(0.54 , 0.46, 0.0 , _order, _n);
            break;
            
        case BLACKMAN:
            ret = cosine_window(0.42 , 0.5, 0.08 , _order, _n);
            break;
            
        default:
            ret = 0;
            break;
    }
    
    return ret;
}




