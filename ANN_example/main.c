#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "ANN.h"

#define NUMBER_INPUT  3
#define NUMBER_OUTPUT 4
/*define parameter*/
pthread_t thr1;

/*declare variable*/

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";

    ANN_LIST* ann = ANN_init(1, NUMBER_INPUT, NUMBER_OUTPUT, LINEAR, 0.5, 0.0);
    
    printf("number of %d layer from list\n",ANN_get_number_of_layer(ann));
    printf("learning rate from index 0 is : %f , mometum rate: %f\n",ANN_get_learning_rate_with_layer_index(ann,0),ANN_get_mometum_rate_with_layer_index(ann,0));
    
    int row=0,col=0;
    ANN_get_row_and_col_with_index(ann,0,&row ,&col);
    double** neurons = ANN_get_neurons_with_index(ann, 0);
    
    for (int i=0; i<col; i++) {
        for (int j=0; j<row; j++) {
            printf("%f ",neurons[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    ANN_set_learning_rate_with_layer_index(ann,0,0.255);
    
    printf("learning rate from index 0 is : %f , mometum rate: %f\n",ANN_get_learning_rate_with_layer_index(ann,0),ANN_get_mometum_rate_with_layer_index(ann,0));
    
    ANN_IO input,expected_output;
    double *array = (double*)malloc(NUMBER_INPUT*sizeof(double));
    array[0]=array[1]=array[2]=1;
    input.io_scale = NUMBER_INPUT;
    input.io_quantity = 1;
    input.io_array = double_array_malloc(NUMBER_INPUT, 1);
    memcpy(input.io_array[0],array,NUMBER_INPUT*sizeof(double));
    
    double *array2 = (double*)malloc(NUMBER_OUTPUT*sizeof(double));
    memset(array2, 1 , NUMBER_OUTPUT);
    expected_output.io_scale = NUMBER_OUTPUT;
    expected_output.io_quantity = 1;
    expected_output.io_array = double_array_malloc(NUMBER_OUTPUT,1);
    memcpy(expected_output.io_array, array2 , NUMBER_OUTPUT);
    //memcpy(input.io_array[0],array,3*sizeof(double));

    printf("start ann algorithm\n");
    ANN_IO output = ANN_algorithm_start(ann, input ,expected_output);
    
    for (int i=0; i<output.io_quantity; i++) {
        for (int j=0; j<output.io_scale; j++) {
            printf("%f ",output.io_array[i][j]);
        }
        printf("\n");
    }
    
    double_array_free(input.io_array);
    double_array_free(output.io_array);
    
    ANN_deinit(ann);

	ret = pthread_create(&thr1,NULL,thread_function,str1);
	if (ret !=0)
	{
		printf("thread create fail");
	}

	pthread_join(thr1,NULL);

	//while loop
	while(1);
	return 0;


}

void *thread_function(void* ptr)
{
	char *str;
	str = (char *)ptr;
	printf("pthread_t:%s\n",str);
    

    
    return 0;
}

