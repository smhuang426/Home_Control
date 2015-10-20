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
void matrix_multi1(int row, int col, double** var1, double* var2, double* output);
void set_double_value_for_array(double* arr, double value,int num);

/*implement function*/
void set_double_value_for_array(double* arr, double value,int num)
{
    int temp_num = num;
    
    while(temp_num)
    {
        arr[--temp_num] = value;
    }
}

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

void ANN_IO_init(int scale, int quantity, ANN_IO* outcome)
{
    double *array = (double*)malloc(scale*sizeof(double));
    set_double_value_for_array(array, 1.0, scale);
    outcome->io_scale = scale;
    outcome->io_quantity = quantity;
    outcome->io_array = double_array_malloc(scale, quantity);
    
    if (outcome->io_array == NULL)
    {
        printf("[%s]allocate fail\n",__func__);
        return;
    }
    
    int i=0;
    for (; i<quantity; i++) {
        memcpy(outcome->io_array[0],array,scale*sizeof(double));
    }
    free(array);
}

void ANN_IO_deinit(ANN_IO* input)
{
    double_array_free(input->io_array);
    input->io_array = NULL;
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
        layer_data->layer_input = NULL;
        
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

void matrix_multi1(int row, int col, double** var1, double* var2, double* output)
{
    int row_index = 0,col_index = 0;
    
    for (col_index = 0; col_index<col; col_index++) {
        for (row_index = 0; row_index<row; row_index++) {
            output[col_index] = output[col_index] + var1[col_index][row_index]*var2[row_index];
        }
    }
    
}

void matrix_multi1_with_transpose(int row, int col, double** var1, double* var2, double* output)
{
    int row_index = 0,col_index = 0;

    for (row_index = 0; row_index<row; row_index++) {
        for  (col_index = 0; col_index<col; col_index++){
            output[row_index] = output[row_index] + var1[col_index][row_index]*var2[col_index];
        }
    }
    
}

void matrix_multi2(int row, int col, double* var1, double* var2, double** output)
{
    int row_index = 0,col_index = 0;
    
    for (col_index = 0; col_index<col; col_index++) {
        for (row_index = 0; row_index<row; row_index++) {
            output[col_index][row_index] = var1[row_index]*var2[col_index];
        }
    }
    
}

void matrix_multi3(int row, double var1, double* var2, double* output)
{
    int row_index = 0;
    
    for (row_index = 0; row_index<row; row_index++) {
        output[row_index] = var1*var2[row_index];
    }
    
}


void ANN_activation_function_start(double* input, double* output,int sizeof_input, uint8_t _type,uint8_t is_backward)
{
    int index = 0;
    for (index = 0 ; index<sizeof_input; index++) {
        output[index] = ANN_activation_function(input[index], _type, is_backward);
    }
}

ANN_IO ANN_algorithm_start(ANN_LIST* list, ANN_IO input, ANN_IO expected_out)
{
    int row = 0,col = 0;
    
    ANN_get_row_and_col_with_index(list, 0 ,&row ,&col);
    
    if (input.io_scale != row)
    {
        printf("input size is not correct %d , %d !\n",input.io_scale,row);
        //return NULL;
    }
    
    DLL_NODE* node = (DLL_NODE*)list->head;
    LAYER* tail_data = (LAYER*)((DLL_NODE*)list->tail)->data;

    ANN_IO output,error;
    error.io_scale = output.io_scale = tail_data->ann_col;
    error.io_quantity = output.io_quantity = input.io_quantity;
    output.io_array = double_array_malloc(output.io_scale, output.io_quantity);
    error.io_array = double_array_malloc(error.io_scale, error.io_quantity);

    if (expected_out.io_scale != output.io_scale)
    {
        printf("expected output scale is not correct\n");
        return output;
    }

    if (input.io_quantity != expected_out.io_quantity)
    {
        printf("input quantity and output quatity is difference\n");
        return output;
    }
    
    printf("output scale:%d quantity:%d \n",output.io_scale,output.io_quantity);
    
    int input_index = 0;
    while (input_index < input.io_quantity) {
        
        ANN_forward_algorithm_start(list, output.io_array[input_index], input.io_array[input_index]);
        ANN_error_calculate(output.io_array[input_index], expected_out.io_array[input_index], error.io_scale, error.io_array[input_index]);
        input_index++;
    }
    
    input_index = 0;
    while (input_index < input.io_quantity) {
        ANN_backward_algorithm_start(list, error.io_array[input_index]);
        input_index++;
    }
    
    return output;
    
}

void double_valuecpy(double * destination ,const double * source, int num)
{
    int temp_num = num,index = 0;
    
    while (temp_num) {
        
        destination[index] = source[index];
        
        temp_num--;
        index++;
    }
}

void ANN_forward_algorithm_start(ANN_LIST* list, double* output, double* input)
{
    double* temp_input ;//input;
    DLL_NODE* node = (DLL_NODE*)list->head;
    uint8_t is_init = 1;
    int num_output = 0;

    while (node != NULL) {
        
        LAYER* data = (LAYER*)node->data;
        
        if (is_init)
        {
            is_init = 0;
            temp_input = (double*)malloc(data->ann_row*sizeof(double));
            if (temp_input == NULL)
            {
                printf("[%s]lack of memory\n",__func__);
                return ;
            }
            double_valuecpy(temp_input, input ,data->ann_row);
        }

        if (data->layer_input == NULL)
        {
            data->layer_input = (double*)malloc(data->ann_row*sizeof(double));
            if (data->layer_input == NULL) {
                printf("[%s]memory is not enough\n",__func__);
                return;
            }
        }
        num_output = data->ann_col;
        double *temp_output = (double*)malloc(num_output*sizeof(double));
        if (temp_output == NULL)
        {
            printf("[%s]lack of memory\n",__func__);
            return ;
        }
        set_double_value_for_array(temp_output, 0.0, num_output);
        
        memcpy(data->layer_input,temp_input,sizeof(double)*data->ann_row);
        
        matrix_multi1(data->ann_row, data->ann_col, data->neurons, temp_input, temp_output);
        
        ANN_activation_function_start(temp_output, temp_output, data->ann_col, data->act_function, 0);
        
        node = node->next;
        
        temp_input = (double*)realloc(temp_input , sizeof(double)*num_output);
        if (temp_input == NULL)
        {
            printf("[%s]lack of memory\n",__func__);
            return ;
        }
        
        memcpy(temp_input,temp_output,sizeof(double)*num_output);

        free(temp_output);
        temp_output = NULL;
    }
    
    memcpy(output, temp_input , sizeof(double)*num_output);
    
    free(temp_input);
    temp_input = NULL;
}

void ANN_error_calculate(double* output, double* expected_output, int size, double* error)
{
    int temp_size = size;

    while(temp_size)
    {
	temp_size--;
	error[temp_size] = expected_output[temp_size] - output[temp_size];
    }
int i =0;
for (i=0;i<size;i++)
{
	printf("error:%f ",error[i]);
}
printf("\n");
}

void ANN_neurons_update(LAYER* layer, double **delta_weight)
{
    int row = layer->ann_row, col = layer->ann_col, row_index = 0 , col_index = 0;
    double lr = layer->learning_rate, mr = layer->mometum_rate;

    if (mr == 0)
    {
        for (col_index = 0; col_index < col; col_index++) {
            for (row_index = 0; row_index < row; row_index++) {
                layer->neurons[col_index][row_index] = layer->neurons[col_index][row_index] - lr*delta_weight[col_index][row_index];
            }
        }
    }else{
        for (col_index = 0; col_index < col; col_index++) {
            for (row_index = 0; row_index < row; row_index++) {
                layer->neurons[col_index][row_index] = layer->neurons[col_index][row_index] - (lr*delta_weight[col_index][row_index] + mr*layer->last_delta_weight[col_index][row_index]);
            }
        }
    }
}

void ANN_backward_algorithm_start(ANN_LIST* list, double* error)
{
    double* temp_out ;
    DLL_NODE* node = (DLL_NODE*)list->tail;
    uint8_t is_init = 1;

    while (1) {
        LAYER* data = (LAYER*)node->data;
        double** delta_w = double_array_malloc(data->ann_row, data->ann_col);
        
        if (delta_w == NULL)
        {
            printf("[%s] delta_w array allocate fail\n",__func__);
            return;
        }
        
        if (is_init == 1)
        {
            is_init = 0;
            
            temp_out = (double*)malloc(data->ann_col*sizeof(double));
            if (temp_out == NULL)
            {
                printf("[%s] temp_out array allocate fail\n",__func__);
                return ;
            }
            set_double_value_for_array(temp_out, 0.0, data->ann_col);
            matrix_multi3(data->ann_col, -1.0, error, temp_out);
        }
        
        //matrix_multi3(data->ann_col, data->learning_rate, temp_out, temp_out);
        ANN_activation_function_start(temp_out, temp_out, data->ann_col, data->act_function, 1);
        matrix_multi2(data->ann_row, data->ann_col, data->layer_input, temp_out, delta_w);
        
        //calculate error for previous layer
        if (node->previous != NULL)
        {
            double *out = (double*)malloc(data->ann_col*sizeof(double));
            set_double_value_for_array(out, 0.0, data->ann_col);
            double_valuecpy(out, temp_out, data->ann_col);
        
            temp_out = (double*)realloc(temp_out , sizeof(double)*data->ann_row);
            if (temp_out == NULL)
            {
                printf("[%s]lack of memory\n",__func__);
                return ;
            }
            set_double_value_for_array(temp_out, 0.0, data->ann_col);
        
            matrix_multi1_with_transpose(data->ann_row, data->ann_col, data->neurons, out, temp_out);
            free(out);
            out = NULL;
        }
        // end of calculate

        //update neurons
        ANN_neurons_update(data, delta_w);
        
        double_array_free(delta_w);
        
        if (node->previous == NULL)
        {
            goto END;
        }

        node = node->previous;
    }
    
END:
    free(temp_out);
    temp_out = NULL;
    
}
