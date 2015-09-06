#include <stdio.h>
#include <stdlib.h>
#include "Link_list.h"


/*implement function*/
LINK_NODE* Link_list_init(void* data)
{
	LINK_NODE* list = (LINK_NODE*)malloc(sizeof(LINK_NODE));
	if (list != NULL)
	{
		list->data = data;
		list->next = NULL;
		return list;
	}
	return NULL;
}

void Link_list_init_with_LinkList(void* data, LINK_NODE** list)
{
	*list = (LINK_NODE*)malloc(sizeof(LINK_NODE));

	if (*list != NULL)
	{
		(*list)->data = data;
		(*list)->next = NULL;
	}
}


void Link_list_destroy(LINK_NODE** list)
{
    while (*list != NULL) {
        Link_list_remove_node(list, *list);
    }
}

LINK_NODE* Link_list_insert_after(LINK_NODE* node, void* data)
{
	LINK_NODE* new_list = Link_list_init(data);

	if (new_list != NULL)
	{
		new_list->next = node->next;
		node->next = new_list;
		return new_list;
	}
	return NULL;
}


LINK_NODE* Link_list_insert_begin(LINK_NODE* list, void* data)
{
	LINK_NODE* new_list = Link_list_init(data);

	if (new_list != NULL)
	{
		new_list->next = list;
		return new_list;
	}
	return NULL;
}


LINK_NODE* Link_list_insert_end(LINK_NODE* list, void* data)
{

    LINK_NODE* new_node = Link_list_get_last_node(list);
    
    new_node->next = Link_list_init(data);

	return list;
	
}


void Link_list_remove_node(LINK_NODE** list, LINK_NODE* node)
{
	if ((*list == NULL)||((*list)->next==NULL)||(node==NULL))
    {
        printf("[remove_node]NULL\n");
        return ;
    }

	if (*list == node)
	{
		*list = (*list)->next;

		free(node);
		node = NULL;
        
	}else{
        
		LINK_NODE* temp = *list;
		while((temp->next != NULL) && (temp->next != node))
        {
            temp = temp->next;
        }

		if (temp->next != NULL)
		{
			temp->next = node->next;
			free(node);
			node = NULL;
		}
	}
}

void Link_list_remove_data(LINK_NODE** list, void* data)
{
	if ((*list == NULL)||((*list)->next==NULL)||(data==NULL)) return ;

	Link_list_remove_node(list, Link_list_find_data(*list, data));

}

LINK_NODE* Link_list_find_node(LINK_NODE* list, LINK_NODE* node)
{
	LINK_NODE *temp = list;
	return temp;
}

LINK_NODE* Link_list_find_data(LINK_NODE* list, void* data)
{
	LINK_NODE *temp = list;
    
	while (temp != NULL)
	{
		if (temp->data == data) break;
        
        temp = temp->next;
	}
	return temp;
}

LINK_NODE* Link_list_get_last_node(LINK_NODE* list)
{
    LINK_NODE* temp = list;
    
    if (temp == NULL)
    {
        return NULL;
    }

    while (temp->next != NULL) {
        temp = temp->next;
    }
    
    return temp;
}

int Link_list_get_size(LINK_NODE* list)
{
	int result = 0;
//printf("data addr :%d\n",*(int*)(list->data));
	LINK_NODE* temp = list;
//printf("data addr :%d\n",*(int*)(list->data));
	while (temp != NULL)
	{
		//printf("data addr :%d\n",*(int*)(list->data));
		temp = temp->next;
		result++;
		//printf("data addr :%d\n",*(int*)(list->data));
	}

	return result;
}

