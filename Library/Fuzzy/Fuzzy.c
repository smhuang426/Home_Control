#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Fuzzy.h"

FUZZIFICATION_LIST* FuzzyList;
FUZZIFICATION_LIST* QuantityList;
/*implement function*/
double FuzzyRecursive(){
    //TODO:
}

double Bell_Function(double input,double a,double b,double c)
{
    return 1 / (1 + pow((input - a)/c, 2*b));
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
