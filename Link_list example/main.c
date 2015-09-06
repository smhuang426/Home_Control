#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#inlcude "../Library/Link_list/Link_list.h"
#include "Link_list.h"

/*define parameter*/
pthread_t thr1;
LINK_NODE* _list;
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
    test1.num = 1;
    strcpy(test1.name,"Noah");
    
    test2.num = 2;
    strcpy(test2.name,"Man");
    
    test3.num = 3;
    strcpy(test3.name,"Nobody");
    
    _list = Link_list_init(&test1);
    Link_list_insert_end(_list, &test2);
    Link_list_insert_end(_list, &test3);
    

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
    
    TEST *c = (TEST*)_list->data;
    LINK_NODE* b = _list->next;
    
    TEST* a = (TEST*)b->data;
    
    printf("test1 ret num:%d ,name:%s\n",c->num,c->name);
    printf("test2 ret num:%d ,name:%s\n",a->num,a->name);
    
    int list_size = Link_list_get_size(_list);
    printf("size of list :%d\n",list_size);

    Link_list_remove_data(&_list, a);
    
    list_size = Link_list_get_size(_list);
    printf("size of list :%d\n",list_size);
    
    TEST* d = (TEST*)_list->data;
    printf("test1 ret num:%d ,name:%s\n",d->num,d->name);

    
    
    return 0;
}

