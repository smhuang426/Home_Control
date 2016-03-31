#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Fuzzy.h"

/*declair parameter*/
FUZZIFICATION_LIST* FuzzyList;
FUZZIFICATION_LIST* QuantityList;

typedef double (*callBack)(double* agrs);
callBack *ack_base;

/*implement function*/
double Bell_Function(double input,double a,double b,double c)
{
    return 1 / (1 + pow((input - a)/c, 2*b));
}

double FuzzyRecursive(DLL_NODE* node, double* input,double* output){
    
    double result;
    int index = DLL_get_index_with_node(FuzzyList,  node);
    Bell *bells = (Bell*)(node->data);
    
    if (node->next != NULL){
        result = Bell_Function(input[index], bells[0].width, bells[0].slope, bells[0].center) * FuzzyRecursive(node->next, input, output);
    }else{
        result = Bell_Function(input[index], bells[0].width, bells[0].slope, bells[0].center);
    }
    
    return result;
    //TODO
}

void Fuzzy_AddBellParametersToInputNumber(int numberOfDataInBell, Bell* bells){

    int *_q;
    if (FuzzyList == NULL){
        *_q = numberOfDataInBell;
        DLL_init_list_with_data(&FuzzyList, bells);
        DLL_init_list_with_data(&QuantityList, _q);
    }else{
        *_q = numberOfDataInBell;
        DLL_insert_data_to_tail(FuzzyList, bells);
        DLL_insert_data_to_tail(QuantityList, _q);
    }
}

void Fuzzy_DefuzzicationInit(){
    
    if (QuantityList != NULL){
        int num_of_callback = 1, iter = 0;
        iter = DLL_get_number_of_data(QuantityList);
        
        DLL_NODE* node = QuantityList->head;
        while (node != NULL) {
            num_of_callback *= *(int*)(node->data);
            node = node->next;
        }
        
        ack_base = (callBack*)realloc(ack_base,num_of_callback*sizeof(callBack));
    }

}

void NormalizationLayer(int numOfOutput, double *output){
    
    int index = 0;
    double total = 0;
    
    for (index = 0; index < numOfOutput; index++){
        total += output[index];
    }
    
    for (index = 0; index < numOfOutput; index++){
        output[index] = output[index] / total;
    }

    
}
