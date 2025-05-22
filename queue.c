#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_node = malloc(sizeof(struct list_head));
    if (new_node == NULL)
        return NULL;
    INIT_LIST_HEAD(new_node);
    return new_node;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head == NULL)
        return;

    struct list_head *curr_node, *next_node;

    curr_node = head->next;

    while (curr_node->next != head) {
        next_node = curr_node->next;
        element_t *entry = list_entry(curr_node, element_t, list);
        if (entry->value) {
            free(entry->value);
            entry->value = NULL;
        }
        free(curr_node);
        curr_node = next_node;
    }
    free(head);
}

/* Insert an element at node's next position of queue */
bool insert_next(struct list_head *node, char *s)
{
    if (node == NULL)
        return false;

    // element_t contains struct list_head, so no need to malloc to list_head
    element_t *new_element = malloc(sizeof(element_t));
    if (new_element == NULL)
        return false;

    if (s != NULL) {
        size_t s_len = strlen(s) + 1;
        new_element->value = malloc(s_len + 1);
        if (new_element->value == NULL) {
            free(new_element);
            return false;
        }
        strncpy(new_element->value, s, s_len);
    } else {
        new_element->value = NULL;
    }

    list_add(&new_element->list, node);

    return true;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    return insert_next(head, s);
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    return insert_next(head->prev, s);
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int len = 0;
    struct list_head *li;

    list_for_each(li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
