#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "D_linked_list.h"

/*define parameter*/
pthread_t thr1;
DLL_LIST* _list=NULL;
/*declare variable*/
typedef struct _TEST{
    int     num;
    char    name[20];
} TEST;

/*declare function*/
void *thread_function(void* ptr);

int main()
{
	int ret = 0;
	char* str1="test1";

	printf("hello world\n");
    
    
    TEST test1,test2,test3;
    test1.num = 5;
    strcpy(test1.name,"Noah");
    
    test2.num = 2;
    strcpy(test2.name,"Man");
    
    test3.num = 3;
    strcpy(test3.name,"Nobody");
    
    DLL_init_list_with_data(&_list, &test1);
    DLL_insert_data_to_tail(_list, &test2);
    //DLL_insert_data_to_index(_list, &test3,1);
    DLL_insert_data_to_head(_list, &test3);
    

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
    
    DLL_NODE* head = _list->head;
    TEST *c = (TEST*)head->data;
    
    DLL_NODE* head_next = head->next;
    TEST* a = (TEST*)head_next->data;
    
    DLL_NODE* tail = _list->tail;
    TEST* d = (TEST*)tail->data;
    
    printf("test1 ret num:%d ,name:%s\n",c->num,c->name);
    printf("test2 ret num:%d ,name:%s\n",a->num,a->name);
    printf("test3 ret num:%d ,name:%s\n",d->num,d->name);

    int list_size =DLL_get_number_of_data(_list);
    printf("size of list :%d\n",list_size);
    
    //DLL_remove_node_with_index(&_list, 1);
    //DLL_remove_head(&_list);
    DLL_remove_tail(&_list);
    
    DLL_NODE* tail2 = _list->tail;
    d = (TEST*)tail2->data;
    printf("test3 ret num:%d ,name:%s\n",d->num,d->name);
    
    list_size =DLL_get_number_of_data(_list);
    printf("size of list :%d\n",list_size);

    DLL_free_list(&_list);
    
    return 0;
}

