#ifndef _PID_H_
#define _PID_H_

typedef struct _INTERATION_PARA{
    float  I_parameter;
    float  I_sum;
} INTERATION_PARA;

typedef struct _DERIVATION_PARA{
    float  D_parameter;
    float  D_last_p;
} DERIVATION_PARA;

typedef struct _PID_PARA{
    float           P_parameter;
    INTERATION_PARA Integ;
    DERIVATION_PARA Derv;
    float           T_time;
} PID_PARA;


typedef struct _CONTROLLER{
    PID_PARA para;
    float Expected_output;
} CONTROLLER;


//declare public function
void PID_setup_controller(CONTROLLER *ctrl, float P_par, float I_par, float D_par , float T_par, float expected_o);
void PID_set_expected_output(CONTROLLER *ctrl, float expected_o);
void PID_set_time_period(CONTROLLER *ctrl, float T_par);
void PID_set_pid_parameter(CONTROLLER *ctrl, float P_par, float I_par, float D_par);
void PID_activate(CONTROLLER *ctrl, float feedback, float *output);
void PID_activate_with_limitation(CONTROLLER *ctrl, float feedback, float *output, float up_limit, float down_limit);

#endif