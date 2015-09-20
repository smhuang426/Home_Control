#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "ANN.h"

/*declare private variable*/
uint8_t has_seed = 0;

/*declare provate function*/
double hyperbolic_tangent(double _input);
double diff_hyperbolic_tangent(double _back_output);
double** double_array_malloc(int row, int col);
void matrix_multi(int row, int col, double** var1, double* var2, double* output);

/*implement function*/
double hyperbolic_tangent(double _input)
{
    return 1.716*(1 - exp (-0.667*_input))/(1 - exp (-0.667*_input));
}

double diff_hyperbolic_tangent(double _back_output)
{
    return 0.667/1.716*(1.716 - _back_output)*(1.716 + _back_output);
}

double logistic_function(double _input)
{
    return 1/(1 + exp(-1*_input));
}

double diff_logistic_function(double _back_output)
{
    return _back_output * (1 - _back_output);
}

double lenear_function(double _input)
{
    return _input;
}

double diff_linear_function(double _back_output)
{
    return _back_output;
}

double ANN_activation_function(double _input, uint8_t _type, uint8_t is_backward)
{
    double ret = 0;
    
    switch (_type) {
        case HYPERBOLIC_TANGENT:
            
            if (is_backward == true)
            {
                ret = diff_hyperbolic_tangent(_input);
            }
            else
            {
                ret = hyperbolic_tangent(_input);
            }
            break;
            
        case LOGISTIC:
            
            if (is_backward == true)
            {
                ret = diff_logistic_function(_input);
            }
            else
            {
                logistic_function(_input);
            }
            
            break;
            
        case LINEAR:
            
            if (is_backward == true)
            {
                ret = diff_linear_function(_input);
            }
            else
            {
                ret = lenear_function(_input);
            }
            
            break;
            
        default:
            break;
    }
    
    return ret;
}

double** ANN_neurons_init(int row, int col)
{
    int index = 0,sub_index = 0;
    double **neurons = (double **)malloc(col*sizeof(double *));
    neurons[0] = (double *)malloc(row*col*sizeof(double));
    
    if (has_seed == 0)
    {
        srand((unsigned)time(NULL));
        has_seed = 1;
    }
    
    for (index = 0; index < col ; index++)
    {
        neurons[index]=(double *)(*neurons+(index*row));
    }
    
    for (index = 0 ; index<col ; index++)
    {
        for (sub_index = 0 ; sub_index<row ;sub_index++)
        {
            neurons[index][sub_index] = ((double)(rand()%10000))/10000;
            //printf("%f ",neurons[index][sub_index]);
        }
        //printf("\n");
    }
    
    return neurons;
}

void ANN_neurons_deinit(double** neurons)
{
    free(neurons);
    neurons = NULL;
}

/*            number of input , activation func, learning rate, mometum rate*/
/*quantity    n+1               n                n              n           */
/*summation  =  4*n+1                                                       */
ANN_LIST* ANN_init(int num_of_layer, ...)
{
    printf("[%s] creating ANN\n",__func__);

    int summation = 4*num_of_layer + 1,index = 0;
    double* temp = (double*)malloc(summation*sizeof(double));
    ANN_LIST* ann_struct = NULL;
    va_list arg_ptr;
    
    //preload data to temp
    memset(temp,0x00,(unsigned long)summation);
    va_start(arg_ptr,num_of_layer);
    
    for (index = 0; index < summation ; index++)
    {
        if (index < 2*num_of_layer+1)
        {
            temp[index] = (double)va_arg(arg_ptr,int);
        }
        else
        {
            temp[index] = va_arg(arg_ptr,double);
        }
        printf("%f\n",temp[index]);
    }
    
    va_end(arg_ptr);
    //preload finish
    
    for (index = 0; index < num_of_layer ;index++)
    {
        LAYER* layer_data = (LAYER*)malloc(sizeof(LAYER));
        
        layer_data->learning_rate = temp[(2*num_of_layer+1) + index];
        layer_data->act_function = (uint8_t)temp[(num_of_layer+1) + index];
        layer_data->mometum_rate = temp[(3*num_of_layer+1) + index];
        layer_data->ann_row = (int)temp[index];  //input
        layer_data->ann_col = (int)temp[index+1];//output
        
        printf("layer%d, learing rate:%f, activation function:%d, mometum rate:%f\n",index+1,layer_data->learning_rate,layer_data->act_function,layer_data->mometum_rate);
        
        layer_data->neurons = ANN_neurons_init(layer_data->ann_row, layer_data->ann_col);
        layer_data->last_delta_weight = ANN_neurons_init(layer_data->ann_row, layer_data->ann_col);
        
        if (ann_struct == NULL)
        {
            DLL_init_list_with_data(&ann_struct, layer_data);
        }
        else
        {
            DLL_insert_data_to_tail(ann_struct, layer_data);
        }
    }
    
    free(temp);
    
    return ann_struct;

}

void ANN_deinit(ANN_LIST* list)
{
    //release neurons
    DLL_NODE* temp = (DLL_NODE*)list->head;
    while(temp != NULL)
    {
        LAYER *data;
        
        data = (LAYER*)temp->data;
        ANN_neurons_deinit(data->neurons);
        ANN_neurons_deinit(data->last_delta_weight);
        
        temp = temp->next;
    }
    
    DLL_free_list(&list);
}

int ANN_get_number_of_layer(ANN_LIST* list)
{
    return DLL_get_number_of_data((DLL_LIST*)list);
}

LAYER* ANN_get_layer_data_with_index(ANN_LIST* list,int index)
{
    int num_of_layer = ANN_get_number_of_layer(list);
    
    if ((num_of_layer-1) < index)
    {
        printf("[%s] error , index is bigger than expected\n",__func__);
        return NULL;
    }
    
    DLL_NODE *temp = DLL_get_node_with_index((DLL_LIST*)list, index);
    return (LAYER*)temp->data;
    
}

double ANN_get_learning_rate_with_layer_index(ANN_LIST* list,int index)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return 0.0;
    
    return data->learning_rate;
}

double ANN_get_mometum_rate_with_layer_index(ANN_LIST* list,int index)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return 0.0;
    
    return data->mometum_rate;
}

uint8_t ANN_get_activation_function_with_index(ANN_LIST* list, int index)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return 0;
    
    return data->act_function;
}

void ANN_get_row_and_col_with_index(ANN_LIST* list, int index ,int* row ,int* col)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return ;
    
    *row = data->ann_row;
    *col = data->ann_col;
}

double** ANN_get_neurons_with_index(ANN_LIST* list, int index)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return NULL;
    
    return data->neurons;
}

double** ANN_get_last_delta_weight_with_index(ANN_LIST* list, int index)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return NULL;

    return data->last_delta_weight;
}

void ANN_set_learning_rate_with_layer_index(ANN_LIST* list,int index, double learn_rate)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return;
    
    data->learning_rate = learn_rate;
}

void ANN_set_mometum_rate_with_layer_index(ANN_LIST* list,int index , double mom_rate)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return;

    data->mometum_rate = mom_rate;
}

void ANN_set_activation_function_with_index(ANN_LIST* list, int index , uint8_t act_fun)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
        
    if (data == NULL) return;
    
    data->act_function = act_fun;
}

void ANN_set_neurons_with_index(ANN_LIST* list, int index , double** neurons)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return;

    data->neurons = neurons;
}

void ANN_set_last_delta_weight_with_index(ANN_LIST* list, int index ,double** del_w)
{
    LAYER* data = ANN_get_layer_data_with_index(list,index);
    
    if (data == NULL) return;

    data->last_delta_weight = del_w;
}

double** double_array_malloc(int row, int col)
{
    int index = 0,sub_index = 0;
    double **array = (double **)malloc(col*sizeof(double *));
    array[0] = (double *)malloc(row*col*sizeof(double));
    
    for (index = 0; index < col ; index++)
    {
        array[index]=(double *)(*array+(index*row));
    }
    
    for (index = 0 ; index<col ; index++)
    {
        for (sub_index = 0 ; sub_index<row ;sub_index++)
        {
            array[index][sub_index] = 0;
        }
    }
    
    return array;
}

void double_array_free(double** double_array)
{
    free(double_array);
    double_array = NULL;
}

void matrix_multi(int row, int col, double** var1, double* var2, double* output)
{
    int row_index = 0,col_index = 0;
    
    for (col_index = 0; col_index<col; col_index++) {
        for (row_index = 0; row_index<row; row_index++) {
            output[col_index] = output[col_index] + var1[col_index][row_index]*var2[row_index];
        }
    }
    
}

void ANN_activation_function_start(double* input, double* output,int sizeof_input, uint8_t _type,uint8_t is_backward)
{
    int index = 0;
    for (index = 0 ; index<sizeof_input; index++) {
        output[index] = ANN_activation_function(input[index], _type, is_backward);
    }
}

ANN_IO* ANN_forward_algorithm_start(ANN_LIST* list, ANN_IO input)
{
    int row = 0,col = 0;
    
    ANN_get_row_and_col_with_index(list, 0 ,&row ,&col);
    
    if (input.io_scale != row)
    {
        printf("input size is not correct !\n");
        return NULL;
    }
    
    int input_index = 0;
    DLL_NODE* node = (DLL_NODE*)list->head;
    LAYER* tail_data = (LAYER*)((DLL_NODE*)list->tail)->data;

    ANN_IO* output;
    output->io_scale = tail_data->ann_col;
    output->io_quantity = input.io_quantity;
    output->io_array = double_array_malloc(output->io_scale, output->io_quantity);
    
    printf("output scale:%d quantity:%d \n",output->io_scale,output->io_quantity);
    
    while (input_index < input.io_quantity) {

        double* temp_input = input.io_array[input_index];
        
        while (node != NULL) {
            
            LAYER* data = (LAYER*)node->data;
            
            matrix_multi(data->ann_row, data->ann_col, data->neurons, temp_input, output->io_array[input_index]);

            ANN_activation_function_start(output->io_array[input_index], output->io_array[input_index], data->ann_col, data->act_function, 0);

            node = node->next;
            
            memcpy(temp_input,output->io_array[input_index],sizeof(double)*data->ann_col);
            
        }
        input_index++;
    }
    
    return output;
    
}

void ANN_backward_algorithm_start(ANN_LIST* list, ANN_IO* output, int sizeof_output, uint8_t is_offline_learning)
{
    
}
