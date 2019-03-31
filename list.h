/*  linked_list.h
 *
 *  Author: Lucas Black
 *  Date:   03/25/2019
 *  Description: Header file for singularly linked list utilities
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*  Linked List `Node` structure
 *    next          - pointer to the next list node
 *    data          - stored integer within the node
 */
typedef struct Node {
    struct Node *next;
    void *data;
} Node;

/*  Linked List `List` structure
 *    nodes         - memory space to hold pointers to Node structs
 *    size          - maximum size of a list
 */
typedef struct List {
    struct Node *head;
    struct Node *tail;
    int size;
    void (*destroy)(void *);
} List;

/* Memory Allocation
 *    new_list()      - returns a pointer to a `List` struct
 *    new_node()      - returns a pointer to a `Node` struct
 */
List *new_list(  );
Node *new_node( void *data );

/* Memory Deallocation
 *    destroy_node()  - free a node
 *    destroy_list()  - free a list
 */
void destroy_node( List *list, Node *node );
void destroy_list( List *list );

/* Linked List Utilities
 *    list_tail()     - returns a pointer to the head of a input list
 *    list_head()     - returns a pointer to the tail of a input list
 *    list_insert()   - insert a node into a list
 *    list_remove()   - remove a node from a list
 *    list_lookup()   - validate there is a member with the input data
 *    list_size()     - size of a list
 *    list_print()    - print the contents of a list
 */
void list_insert_next( List *list, Node *node, void *data );
void list_remove( List *list, Node *node );
void list_print( List *list );

#define list_size(list)              ((list)->size)
#define list_head(list)              ((list)->head)
#define list_tail(list)              ((list)->tail)
#define list_is_head(list, element)  ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element)        ((element)->next == NULL ? 1 : 0)
#define list_data(element)           ((element)->data)
#define list_next(element)           ((element)->next)

#endif
