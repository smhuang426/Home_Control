#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "FIR.h"

#define PI        3.14159265358979323846

/*implement function*/

/*4 type of filter as below*/
void double_fill(double* source, double val, int size)
{
    int temp_size = size;
    
    while (temp_size) {
        source[--temp_size] = val;
    }
}

double h_low_pass(int _fs,int _fc, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc = (double)_fc;
    double M = (order*0.5);
    double Wc = 2*PI*fc/fs;
    //printf("n=%f,M=%f,Wc=%f\n",n,M,Wc);
    double ret = 0.0;
    
    if (M == n)
    {
        ret = Wc/PI;
    }else{
        ret = sin(Wc*(n-M))/(PI * (n - M));
    }
    return ret;
}

double h_high_pass(int _fs,int _fc, int _order, int _n)
{
    double n = (double)_n,order = (double)_order,fs = (double)_fs, fc = (double)_fc;
    double M = (order*0.5);
    double Wc = 2*PI*fc/fs;
    //printf("n=%f,M=%f,Wc=%f\n",n,M,Wc);
    double ret = 0.0;
    
    if (M == n)
    {
        ret = 1 - (Wc/PI);
    }else{
        ret = -1*sin(Wc*(n-M))/(PI * (n - M));
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
        ret = (sin(Wc2*(n-M))/(PI * (n - M))) - (sin(Wc1*(n-M))/(PI * (n - M)));
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
        ret = (sin(Wc1*(n-M))/(PI * (n - M))) - (sin(Wc2*(n-M))/(PI * (n - M)));
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
            //printf("hi\n\n\n");
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
    if ((n>=0) && (n <= ((order)*0.5) ))
    {
        ret = (2*n)/(order );
    }else if (( n>=((order+1)*0.5) ) && ( n<= (order )))
    {
        ret = 2 - (2*n)/(order );
    }else{
        return 0;
    }
    
    return ret;
}

double cosine_window(double A_para, double B_para,double C_para, int _order, int _n)
{
    double order = (double)_order, n = (double)_n;
 
    if ((n<0) || (n>(order)))
    {
        return 0;
    }
    
    double ret = 0.0;
    
    ret = A_para - B_para*cos((2*PI*n)/(order)) + C_para*cos((4*PI*n)/(order));

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

void FIR_init(FIR_CONFIG* config, uint8_t filter_type, ...)
{
    va_list arg_ptr;
    int num_of_data = 0,index = 0;
    
    config->filter_type = filter_type;
    
    switch (filter_type) {
        case LOWPASS:
        case HIGHPASS:
            config->fc2 = 0;
            num_of_data = 4;
            break;
            
        case BANDPASS:
        case BANDSTOP:
            num_of_data = 5;
            
        default:
            break;
    }

    //preload data to temp
    va_start(arg_ptr,filter_type);
    
    for (index = 0; index < num_of_data ; index++)
    {
        
        switch (index) {
            case 0:
                config->window_type = (uint8_t)va_arg(arg_ptr,int);
                break;
                
            case 1:
                config->order = va_arg(arg_ptr,int);
                break;
                
            case 2:
                config->fs = va_arg(arg_ptr,int);
                break;
                
            case 3:
                config->fc1 = va_arg(arg_ptr,int);
                break;
                
            case 4:
                config->fc2 = va_arg(arg_ptr,int);
                break;
                
            default:
                break;
        }
        
    }
    va_end(arg_ptr);
    
    printf("fir_type:%d ,win_type:%d ,order:%d ,fs:%d ,fc1:%d, fc2:%d\n",config->filter_type,config->window_type,config->order,config->fs,config->fc1,config->fc2);
    
    config->coeficient = (double*)malloc((config->order+1)*sizeof(double));
    if (config->coeficient == NULL)
    {
        return ;
    }
    //memset(config->coeficient,0x00,config->order+1);
    double_fill(config->coeficient, 0.0, config->order+1);
    
    config->input = (double*)malloc((config->order+1)*sizeof(double));
    if (config->input == NULL)
    {
        return ;
    }
    //memset(config->input,0x00,config->order+1);
    double_fill(config->input, 0.0, config->order+1);
    
    printf("the result of coeficient :\n");
    for (index = 0; index < (config->order+1) ; index++)
    {
        config->coeficient[index] = FIR_window_function(config->window_type, config->order, index) * FIR_filter_function(filter_type, config, index);
        printf("%f ",config->coeficient[index]);
    }
    printf("\n");

}

double FIR_start_filter(FIR_CONFIG* config, double input)
{
    int index = config->order;
    double output = 0.0;
    
    //update input array
    while (index >= 0) {
        
        if (index == 0)
        {
            config->input[index] = input;
            //printf("config->input:%f\n",config->input[index]);
        }
        else
        {
            config->input[index] = config->input[index-1];
            //printf("config->input:%f\n",config->input[index]);
        }
        //output = output + config->input[index] * config->coeficient[index];
        index--;
    }
    
    int num_of_array = config->order ;
    index = 0;
    
    while (1) {
        
        switch (num_of_array - (2*index)) {
            case 0:
                output = output + (config->input[index] * config->coeficient[index]);
                
            case -1:
                return output;
                break;
                
            default:
                output = output + ((config->input[index] + config->input[num_of_array - index]) * config->coeficient[index]);
                break;
        }
        
        index++;
    }
    
    return output;
}


