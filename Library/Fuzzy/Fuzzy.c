#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Fuzzy.h"

FUZZIFICATION_LIST* FuzzyList;
FUZZIFICATION_LIST* QuantityList;
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
    //TODO:
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
