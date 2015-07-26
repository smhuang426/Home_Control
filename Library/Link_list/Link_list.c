#include <stdio.h>
#include <stdlib.h>
#include "Link_list.h"


/*implement function*/
LINK_NODE* Link_list_init(NETWORK_CONFIG* data)
{
	LINK_NODE* list = (LINK_NODE*)malloc(sizeof(LINK_NODE));
	if (list != NULL)
	{
		list->data = data;
		list->fd = data->fd;
		list->next = NULL;
		return list;
	}
	return NULL;
}

void Link_list_init_with_LinkList(NETWORK_CONFIG* data, LINK_NODE* list)
{
	list = (LINK_NODE*)malloc(sizeof(LINK_NODE));

	if (list != NULL)
	{
		printf("initailizing\n");
		list->fd = 4;//data->fd;
		//list->data->client_addr = data->client_addr;
		list->next = NULL;
		printf("end of initializing\n");
	}
}


void Link_list_destroy(LINK_NODE* list)
{
	
}

LINK_NODE* Link_list_insert_after(LINK_NODE* node, NETWORK_CONFIG* data)
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


LINK_NODE* Link_list_insert_begin(LINK_NODE* list, NETWORK_CONFIG* data)
{
	LINK_NODE* new_list = Link_list_init(data);

	if (new_list != NULL)
	{
		new_list->next = list;
		return new_list;
	}
	return NULL;
}


LINK_NODE* Link_list_insert_end(LINK_NODE* list, NETWORK_CONFIG* data)
{
	LINK_NODE* new_node = list;

	if (new_node = NULL)
		return NULL;

	while(new_node->next != NULL)
	{
		new_node = new_node->next;
	}
	((LINK_NODE*)(new_node->next))->data = data;
	((LINK_NODE*)(new_node->next))->next = NULL;

	return list;
	
}


void Link_list_remove_node(LINK_NODE* list, LINK_NODE* node)
{
	if ((list == NULL)||(list->next==NULL)||(node==NULL)) return ;

	if (list == node)
	{
		list = list->next;
		free(node);
		node = NULL;
	}else{
		LINK_NODE* temp = list;
		while((temp->next != NULL) && (temp->next != node)) temp = temp->next;

		if (temp->next != NULL)
		{
			temp->next = node->next;
			free(node);
			node = NULL;
		}
	}
}

void Link_list_remove_data(LINK_NODE* list, NETWORK_CONFIG* data)
{
	if ((list == NULL)||(list->next==NULL)||(data==NULL)) return ;

	Link_list_remove_node(list, Link_list_find_data(list, data));

}

LINK_NODE* Link_list_find_node(LINK_NODE* list, LINK_NODE* node)
{
	LINK_NODE *temp = list;
	return temp;
}

LINK_NODE* Link_list_find_data(LINK_NODE* list, NETWORK_CONFIG* data)
{
	LINK_NODE *temp = list;
	while (temp != NULL)
	{
		if (temp->data == data)
		break;
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
		printf("data addr :%d\n",*(int*)(list->data));
		temp = temp->next;
		result++;
		printf("data addr :%d\n",*(int*)(list->data));
	}

	return result;
}

