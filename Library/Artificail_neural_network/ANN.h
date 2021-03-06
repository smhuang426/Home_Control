#ifndef _ANN_H_
#define _ANN_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "D_linked_list.h"

/*struct define*/
typedef struct LAYER{
    int         ann_row;
    int         ann_col;
    double**    neurons;
    double**    last_delta_weight;
    uint8_t     act_function;
    double      learning_rate;
    double      mometum_rate;
    double*     layer_input;
} LAYER;

typedef struct ANN_IO
{
    int         io_scale;
    int         io_quantity;
    double**    io_array;
} ANN_IO;

typedef enum ACTIVATE_FUNCTION{
    HYPERBOLIC_TANGENT = 1,
    LOGISTIC,
    LINEAR
} ACTIVATE_FUNCTION;

typedef DLL_LIST ANN_LIST;

/*global function define*/
ANN_LIST* ANN_init(int num_of_layer, ...);
void ANN_deinit(ANN_LIST* list);
void ANN_IO_init(int scale, int quantity, ANN_IO* outcome);
void ANN_IO_deinit(ANN_IO* input);

int ANN_get_number_of_layer(ANN_LIST* list);
double ANN_get_learning_rate_with_layer_index(ANN_LIST* list,int index);
double ANN_get_mometum_rate_with_layer_index(ANN_LIST* list,int index);
uint8_t ANN_get_activation_function_with_index(ANN_LIST* list, int index);
void ANN_get_row_and_col_with_index(ANN_LIST* list, int index ,int* row ,int* col);
double** ANN_get_neurons_with_index(ANN_LIST* list, int index);
double** ANN_get_last_delta_weight_with_index(ANN_LIST* list, int index);

void ANN_set_learning_rate_with_layer_index(ANN_LIST* list,int index, double learn_rate);
void ANN_set_mometum_rate_with_layer_index(ANN_LIST* list,int index , double mom_rate);
void ANN_set_activation_function_with_index(ANN_LIST* list, int index , uint8_t act_fun);
void ANN_set_neurons_with_index(ANN_LIST* list, int index , double** neurons);
void ANN_set_last_delta_weight_with_index(ANN_LIST* list, int index ,double** del_w);

ANN_IO ANN_algorithm_start(ANN_LIST* list, ANN_IO input, ANN_IO expected_out);
void ANN_forward_algorithm_start(ANN_LIST* list, double* output, double* input);
void ANN_error_calculate(double* output, double* expected_output, int size, double* error);
void ANN_backward_algorithm_start(ANN_LIST* list, double* error);
double** double_array_malloc(int row, int col);
void double_array_free(double** double_array);
void set_double_value_for_array(double* arr, double value,int num);

#endif
