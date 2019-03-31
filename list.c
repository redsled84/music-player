#include "list.h"

List *new_list( void (*destroy)(void *data) )
{

    List *list = calloc(1, sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->destroy = destroy;

    return list;
}

Node *new_node( void *data )
{
    Node *node = calloc(1, sizeof(Node));

    node->data = data;

    return node;
}


void destroy_node( List *list, Node *node )
{
    list->destroy(node->data);
    free(node);
}

void destroy_list( List *list )
{

    Node *node = list_head(list);
    while (node != NULL) {
        Node *temp = node;
        node = node->next;
        destroy_node(list, temp);
    }

    free(list);
    memset(list, 0, sizeof(List));
}

void list_insert_next( List *list, Node *node, void *data )
{
    Node *node_to_insert = new_node(data);
    if (node == NULL) {
        if (list->size == 0)
            list->tail = node_to_insert;
        node_to_insert->next = list->head;
        list->head = node_to_insert;
    } else {
        if (node->next == NULL)
            list->tail = node_to_insert;
        node_to_insert->next = node->next->next;
        node->next = node_to_insert;
    }
    list->size++;
}

void list_remove( List *list, Node *node )
{
    Node *head = list_head(list);
    Node *prev = NULL;
    /* walk down the linked list */
    while (head != NULL) {
        /* matching node to remove is the head */
        if (head == node && prev == NULL) {
            list->head = head->next;
            destroy_node(list, head);
            break;
        /* matching node is tail or in the middle of the list */
        } else if (head == node) {
            prev->next = head->next;
            destroy_node(list, head);
            break;
        }
        prev = head;
        head = head->next;
    }

    list->size--;
}

void list_print( List *list )
{
    Node *head = list_head(list);

    printf("Begin printing list...\n\n");

    if (head == NULL) {
        printf("NO LIST CONTENTS\n");
    }
    int count = 0;
    int size = list_size(list);
    while (head != NULL) {
        if (count < 8 || count > size - 9)
            printf("List Element %4d: Current = %p, Next = %p\n", count, head, head->next);
        head = head->next;
        count++;
    }
    printf("\n...End printing list.\n\n");
}
