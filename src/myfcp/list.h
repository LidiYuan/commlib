#ifndef _MYLIB4C_LIST_H_
#define _MYLIB4C_LIST_H_

#include <stddef.h>

struct list_head
{
	struct list_head *prev;
    struct list_head *next;
};

#define INIT_LIST_HEAD_VAR(name) { &(name), &(name) }


#define INIT_LIST_HEAD_PTR(name_ptr)       \
do{                                   \
    (name_ptr)->next = (name_ptr);         \
	(name_ptr)->prev = (name_ptr);     \
}while(0)


#define OFFSET(type, member)            (char *)&(((type *)0x0)->member)
#define container_of(ptr, type, member) ({(type *)((char * )ptr - OFFSET(type, member)); });

#define list_for_each(pos, head)        for (pos = (head)->next; pos != (head); pos = pos->next)
#define list_for_each_prev(pos, head)   for (pos = (head)->prev; pos != (head); pos = pos->prev)
#define list_entry(ptr, type, member)   container_of(ptr, type, member)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
			pos = n, n = pos->next)


static inline void __list_add(struct list_head *new_node,struct list_head *prev,struct list_head *next)
{
	next->prev = new_node;
	new_node->next = next;
	new_node->prev = prev;
	prev->next = new_node;
}

static inline void list_add_tail(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head->prev, head);
}

static inline void list_add(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head, head->next);
}

static inline void list_del(struct list_head *p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

static inline void list_replace(struct list_head *old,struct list_head *new_node)
{
	new_node->next = old->next;
	new_node->next->prev = new_node;
	new_node->prev = old->prev;
	new_node->prev->next = new_node;
}

static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}


static inline struct list_head *list_pop_front(struct list_head *head)
{
    struct list_head *pos;	
    if( list_empty(head) )
	    return NULL;

    pos = head->next;
    list_del(pos);
    return pos; 
}


static inline struct list_head *list_front(struct list_head *head)
{
    struct list_head *pos;	
    if( list_empty(head) )
	    return NULL;

    pos = head->next; 
    return pos; 
}

static inline int list_size(struct list_head *head)
{
    struct list_head *pos,*next;	
    int sum = 0;
    
    list_for_each_safe(pos,next,head)
    {
        sum++;
    }

    return sum;
}


#endif


