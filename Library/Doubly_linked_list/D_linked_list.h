#ifndef _DOUBLY_LINKED_LIST_H_
#define _DOUBLY_LINKED_LIST_H_


typedef struct DLL_NODE{
    struct DLL_NODE*   previous;
    struct DLL_NODE*   next;
    void*       data;
} DLL_NODE;

typedef struct DLL_LIST{
    DLL_NODE*   head;
    DLL_NODE*   tail;
    int         num_node;
} DLL_LIST;

typedef enum DDL_RESULT{
    DLL_TRUE = 0x00,
    DLL_FALSE
} DLL_RESULT;


/*declare public function*/
DLL_RESULT DLL_init_list(DLL_LIST** list);
DLL_RESULT DLL_init_list_with_data(DLL_LIST** list, void* data);
DLL_RESULT DLL_insert_data_to_tail(DLL_LIST* list, void* data);
DLL_RESULT DLL_insert_data_to_head(DLL_LIST* list, void* data);
DLL_RESULT DLL_insert_data_to_index(DLL_LIST* list, void* data,int index);
DLL_NODE*  DLL_get_node_with_index(DLL_LIST* list, int index);
DLL_RESULT DLL_remove_node(DLL_LIST** list,DLL_NODE* node);
DLL_RESULT DLL_remove_tail(DLL_LIST** list);
DLL_RESULT DLL_remove_head(DLL_LIST** list);
DLL_RESULT DLL_remove_node_with_index(DLL_LIST** list, int index);
int DLL_get_index_with_data(DLL_LIST* list, void* data);
int DLL_get_index_with_node(DLL_LIST* list, DLL_NODE* node);
int DLL_get_number_of_data(DLL_LIST* list);
void DLL_free_list(DLL_LIST** list);

#endif