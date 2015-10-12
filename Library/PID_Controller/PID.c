#include "PID.h"


float PID_Proportion(PID_PARA para, float input);
float PID_Integration(PID_PARA para, float input);
float PID_Derivation(PID_PARA para, float input);
float PID_Controller(PID_PARA para, float input);

float PID_Proportion(PID_PARA para, float input)
{
    return input;
}

float PID_Integration(PID_PARA para, float input)
{
    para.Integ.I_sum = para.Integ.I_sum + (input * para.T_time);
    
    return para.I_sum;
}

float PID_Derivation(PID_PARA para, float input)
{
    float ret = (input - para.Derv.D_last_p)/para.T_time;
    para.Derv.D_last_p = input;
    
    return ret;
}

float PID_Controller(PID_PARA para, float input)
{
    float ret = 0;
    
    ret =   PID_Proportion(para,input) * para.P_parameter + \
            PID_Integration(para,input) * para.Integ.I_parameter + \
            PID_Derivation(para,input) * para.Derv.D_parameter;
    
    return ret;
}

void PID_setup_controller(CONTROLLER *ctrl, float P_par, float I_par, float D_par , float T_par, float expected_o)
{
    ctrl->Para.P_parameter = P_par;
    ctrl->Para.Integ.I_parameter = I_par;
    ctrl->Para.Derv.D_parameter = D_par;
    ctrl->Para.T_time = T_par;
    ctrl->Expected_output = expected_o;
}

void PID_set_expected_output(CONTROLLER *ctrl, float expected_o)
{
    ctrl->Expected_output = expected_o;
}

void PID_set_time_period(CONTROLLER *ctrl, float T_par)
{
    ctrl->Para.T_time = T_par;
}

void PID_activate(CONTROLLER *ctrl, float feedback, float *output)
{
    float err = ctrl->Expected_output - feedback ;
    
    *output = PID_Controller(ctrl->Para, err);
    
}

void PID_activate_with_limitation(CONTROLLER *ctrl, float feedback, float *output, float up_limit, float down_limit)
{
    float err = ctrl->Expected_output - feedback , temp_out = 0;
    
    //normalization
    err = err/ctrl->Expected_output;
    
    temp_out = PID_Controller(ctrl->Para, err);
    
    if (temp_out > up_limit)
    {
        temp_out = up_limit;
    }
    else if (temp_out < down_limit)
    {
        temp_out = down_limit;
    }
    
    //de-normalization
    *output = ctrl->Expected_output*temp_out;
}

