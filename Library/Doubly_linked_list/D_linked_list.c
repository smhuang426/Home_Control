#include "D_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

/*declare private variable*/

/*declare private function*/
DLL_NODE* DLL_init_node_with_data(void* data);

/*implement function*/
DLL_NODE* DLL_init_node_with_data(void* data)
{
    DLL_NODE* node = (DLL_NODE*)malloc(sizeof(DLL_NODE));
    
    node->next = NULL;
    node->previous = NULL;
    node->data = data;
    
    return node;
    
}

DLL_RESULT DLL_init_list_with_data(DLL_LIST** list, void* data)
{
    
    if (*list == NULL)
    {
        *list = (DLL_LIST*)malloc(sizeof(DLL_LIST));
        
        if (*list == NULL)
        {
            printf("[%s] allocating memory of list is fail\n",__func__);
            return DLL_FALSE;
        }
    }
    
    (*list)->head = DLL_init_node_with_data(data);
    
    if ((*list)->head == NULL)
    {
        printf("[%s] allocating memory of node is fail\n",__func__);
        return DLL_FALSE;

    }
    
    (*list)->tail = (*list)->head;
    
    (*list)->num_node = 1;
    
    return DLL_TRUE;
    
}

DLL_RESULT DLL_init_list(DLL_LIST** list)
{
    return DLL_init_list_with_data(list, NULL);
}

DLL_RESULT DLL_insert_data_to_tail(DLL_LIST* list, void* data)
{
    if ((list == NULL) || (data == NULL))
    {
        printf("[%s] input data is NULL, abort\n",__func__);
        return DLL_FALSE;
    }
    
    DLL_NODE* temp_tail = (DLL_NODE*)list->tail;
    
    DLL_NODE* new_node = DLL_init_node_with_data(data);
    if (new_node == NULL) return DLL_FALSE;
    
    if (temp_tail->previous == NULL)
    {
        DLL_NODE* temp_head = (DLL_NODE*)list->head;
        
        temp_head->next = new_node;
        new_node->previous = temp_head;
        
        list->tail = new_node;
        
    }else{
        
        new_node->previous = temp_tail;
        
        temp_tail->next = new_node;
        
        list->tail = new_node;
    }
    
    list->num_node++;
    
    return DLL_TRUE;
}

DLL_RESULT DLL_insert_data_to_head(DLL_LIST* list, void* data)
{
    if ((list == NULL) || (data == NULL))
    {
        printf("[%s] input data is NULL, abort\n",__func__);
        return DLL_FALSE;
    }
    
    DLL_NODE* temp_head = (DLL_NODE*)list->head;
    
    DLL_NODE* new_node = DLL_init_node_with_data(data);
    if (new_node == NULL) return DLL_FALSE;


    new_node->next = temp_head;
        
    temp_head->previous = new_node;
        
    list->head = new_node;
    
    list->num_node++;
    
    return DLL_TRUE;
}

DLL_RESULT DLL_insert_data_to_index(DLL_LIST* list, void* data,int index)
{
    DLL_NODE* temp = DLL_get_node_with_index(list, index);
    DLL_NODE* temp_prev = temp->previous;
    
    if (temp_prev == NULL)
    {
        printf("[%s] this index is head, abort\n",__func__);
        return DLL_FALSE;
    }
    
    DLL_NODE* new_node = DLL_init_node_with_data(data);
    
    if (new_node == NULL)
    {
        printf("[%s] memory is not enough to allocate\n",__func__);
        return DLL_FALSE;
    }
    
    
    temp_prev->next = new_node;
    
    new_node->previous = temp_prev;
    new_node->next = temp;
    
    temp->previous = new_node;
    
    list->num_node++;
    
    return DLL_TRUE;
}

DLL_NODE* DLL_get_node_with_index(DLL_LIST* list, int index)
{
    if (index >= list->num_node)
    {
        printf("[%s] index you set is bigger than number of node\n",__func__);
        return NULL;
    }
    
    int i = 0;
    DLL_NODE* temp = list->head;
    
    while (i<index)
    {
        temp = temp->next;
        i++;
    }
    
    return temp;
}

DLL_RESULT DLL_remove_node(DLL_LIST** list,DLL_NODE* node)
{
    if ((*list == NULL) || (node == NULL))
    {
        printf("[%s] input data is NULL, abort\n",__func__);
        return DLL_FALSE;
    }
    printf("remove1\n");
    if (((DLL_NODE*)(*list)->head)->next == NULL)
    {
        //free((*list)->head);
        //free((*list)->tail);
        free(*list);
        *list = NULL;
        //(*list)->head = NULL;
        //(*list)->tail = NULL;
        
        printf("[%s] list has destroied\n",__func__);

        
        return DLL_TRUE;
    }
    printf("remove2\n");
    
    DLL_NODE* temp_prev = node->previous;
    DLL_NODE* temp_next = node->next;
    
    if (temp_prev == NULL)
    {
        (*list)->head = temp_next;
        temp_next->previous = temp_prev;
    }
    else if (temp_next == NULL)
    {
        (*list)->tail = temp_prev;
        temp_prev->next = temp_next;
    }
    else
    {
        temp_prev->next = temp_next;
        temp_next->previous = temp_prev;
    }
    
    free(node);
    node = NULL;
    
    (*list)->num_node--;
    
    return DLL_TRUE;
    
}

DLL_RESULT DLL_remove_tail(DLL_LIST** list)
{
    return DLL_remove_node(list,(*list)->tail);
}

DLL_RESULT DLL_remove_head(DLL_LIST** list)
{
    return DLL_remove_node(list,(*list)->head);
}

DLL_RESULT DLL_remove_node_with_index(DLL_LIST** list, int index)
{
    DLL_NODE* temp = DLL_get_node_with_index(*list, index);
    if (temp == NULL)
    {
        return DLL_FALSE;
    }
    
    return DLL_remove_node(list, temp);
}

int DLL_get_index_with_data(DLL_LIST* list, void* data)
{
    int index = 0;
    
    DLL_NODE* temp = list->head;
    
    while (temp != NULL) {
        
        if (temp->data == data)
        {
            return index;
        }
        
        temp = temp->next;
        index++;
    }
    
    return index;
}

int DLL_get_index_with_node(DLL_LIST* list, DLL_NODE* node)
{
    return DLL_get_index_with_data(list, node->data);
}

int DLL_get_number_of_data(DLL_LIST* list)
{
    return list->num_node;
}

void DLL_free_list(DLL_LIST** list)
{
    while (*list != NULL)
    {
        DLL_remove_tail(list);
    }

}

