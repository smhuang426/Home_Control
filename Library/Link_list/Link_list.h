#ifndef _LINK_LIST_H_
#define _LINK_LIST_H_
#include "../Network/Network.h"

/*struct define*/
typedef struct LINK_NODE{
	NETWORK_CONFIG 		*data;
	int			fd;
	
	struct LINK_NODE 	*next;
} LINK_NODE;


/*global function define*/
LINK_NODE* Link_list_init(NETWORK_CONFIG* data);
void Link_list_init_with_LinkList(NETWORK_CONFIG* data, LINK_NODE* list);
void Link_list_destroy(LINK_NODE* list);
LINK_NODE* Link_list_insert_after(LINK_NODE* node, NETWORK_CONFIG* data);
LINK_NODE* Link_list_insert_begin(LINK_NODE* list, NETWORK_CONFIG* data);
LINK_NODE* Link_list_insert_end(LINK_NODE* list, NETWORK_CONFIG* data);
void Link_list_remove_node(LINK_NODE* list, LINK_NODE* node);
void Link_list_remove_data(LINK_NODE* list, NETWORK_CONFIG* data);
LINK_NODE* Link_list_find_node(LINK_NODE* list, LINK_NODE* node);
LINK_NODE* Link_list_find_data(LINK_NODE* list, NETWORK_CONFIG* data);
int Link_list_get_size(LINK_NODE* list);

#endif
