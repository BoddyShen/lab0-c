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

    while (curr_node != head) {
        next_node = curr_node->next;
        element_t *entry = list_entry(curr_node, element_t, list);
        if (entry->value) {
            free(entry->value);
            entry->value = NULL;
        }
        free(entry);
        curr_node = next_node;
    }
    free(head);
}

/* Insert an element at node's next position of queue */
bool insert_next_helper(struct list_head *node, char *s)
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
    return insert_next_helper(head, s);
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }

    return insert_next_helper(head->prev, s);
}

/* Remove an element at node's next position of queue */
element_t *remove_next_helper(struct list_head *node, char *sp, size_t bufsize)
{
    if (!node || node->next == node)
        return NULL;
    element_t *first_element = list_entry(node->next, element_t, list);
    if (sp && bufsize > 0) {
        if (first_element->value) {
            strncpy(sp, first_element->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        } else {
            sp[0] = '\0';
        }
    }

    list_del(node->next);
    return first_element;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    return remove_next_helper(head, sp, bufsize);
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || head == head->prev)
        return NULL;
    return remove_next_helper(head->prev->prev, sp, bufsize);
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
    if (!head || head->next == head)
        return false;

    struct list_head *slow = head, *fast = head->next;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    struct list_head *removed_node = slow->next;
    element_t *removed_entry = list_entry(removed_node, element_t, list);
    list_del(removed_node);
    free(removed_entry->value);
    free(removed_entry);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *prev_anchor = head;

    while (prev_anchor->next != head && prev_anchor->next->next != head) {
        const element_t *next_entry =
            list_entry(prev_anchor->next, element_t, list);
        const element_t *next_next_entry =
            list_entry(prev_anchor->next->next, element_t, list);

        if (strcmp(next_entry->value, next_next_entry->value) == 0) {
            char *value_to_delete = strdup(next_entry->value);
            if (!value_to_delete)
                return false;

            struct list_head *curr = prev_anchor->next;
            while (curr != head) {
                element_t *removed_entry = list_entry(curr, element_t, list);
                if (strcmp(removed_entry->value, value_to_delete) == 0) {
                    struct list_head *next_node = curr->next;
                    list_del(curr);
                    curr = next_node;
                    if (removed_entry->value) {
                        free(removed_entry->value);
                        removed_entry->value = NULL;
                    }
                    free(removed_entry);
                } else {
                    break;
                }
            }
            free(value_to_delete);
        } else {
            prev_anchor = prev_anchor->next;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || q_size(head) <= 1)
        return;

    struct list_head *prev = head, *curr = head->next;
    while (curr != head && curr->next != head) {
        struct list_head *next_node = curr->next;
        list_del(next_node);
        list_add(next_node, prev);
        prev = curr;
        curr = curr->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *curr = head->next;
    while (curr != head) {
        struct list_head *next_node = curr->next;
        list_move(curr, head);
        curr = next_node;
    }

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || k <= 1)
        return;
    struct list_head *curr = head->next;

    while (curr != head) {
        int counter = 0;
        struct list_head *prev_group_tail = curr->prev;

        // Find the next head
        struct list_head *new_head = curr;
        while (new_head != head && counter < k) {
            new_head = new_head->next;
            counter++;
        }
        if (counter < k)
            break;

        while (curr != head && curr != new_head) {
            struct list_head *next_node = curr->next;
            list_del(curr);
            list_add(curr, prev_group_tail);
            curr = next_node;
        }

        curr = new_head;
    }
    return;
}

/**
 * Helper function to merge two sorted linear doubly-linked lists.
 * list1 and list2 are pointers to the 'list' member of the first element_t
 * of their respective lists.
 * Returns a pointer to the 'list' member of the first element_t of the merged,
 * sorted, linear doubly-linked list.
 */
struct list_head *merge_sorted_linear_lists(struct list_head *list1,
                                            struct list_head *list2,
                                            bool descend)
{
    if (!list1) {
        return list2;
    }
    if (!list2) {
        return list1;
    }

    struct list_head
        dummy_sentinel;  // Temporary sentinel for building the merged list
    struct list_head *merged_tail = &dummy_sentinel;
    // dummy_sentinel.next will point to the head of the merged list

    while (list1 && list2) {
        const element_t *entry1 = list_entry(list1, element_t, list);
        const element_t *entry2 = list_entry(list2, element_t, list);
        int cmp_result = strcmp(entry1->value, entry2->value);
        bool take_list1_node;

        if (descend) {
            take_list1_node = (cmp_result >= 0);
        } else {
            take_list1_node = (cmp_result <= 0);
        }

        if (take_list1_node) {
            merged_tail->next = list1;
            list1->prev = merged_tail;
            list1 = list1->next;
        } else {
            merged_tail->next = list2;
            list2->prev = merged_tail;
            list2 = list2->next;
        }
        merged_tail = merged_tail->next;
    }

    // Attach the remaining part of whichever list is not exhausted
    if (list1) {
        merged_tail->next = list1;
        list1->prev = merged_tail;
    } else if (list2) {
        merged_tail->next = list2;
        list2->prev = merged_tail;
    }

    struct list_head *merged_list_first_node = dummy_sentinel.next;
    if (merged_list_first_node) {
        merged_list_first_node->prev =
            NULL;  // Ensure the merged list starts with prev = NULL
    }
    return merged_list_first_node;
}

/**
 * Recursive helper to sort a linear doubly-linked list.
 * first_node is a pointer to the 'list' member of the first element_t.
 * Returns a pointer to the 'list' member of the first element_t of the sorted
 * list.
 */
struct list_head *sort_linear_list_recursive(struct list_head *first_node,
                                             bool descend)
{
    if (!first_node || !first_node->next) {
        return first_node;
    }

    struct list_head *slow = first_node;
    struct list_head *fast = first_node->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 'slow' is now the last node of the first half.
    struct list_head *second_half_first_node =
        slow->next;  // Start of the second half

    slow->next = NULL;
    if (second_half_first_node) {
        second_half_first_node->prev = NULL;
    }

    // Recursively sort both halves
    struct list_head *sorted_left_half =
        sort_linear_list_recursive(first_node, descend);
    struct list_head *sorted_right_half =
        sort_linear_list_recursive(second_half_first_node, descend);

    return merge_sorted_linear_lists(sorted_left_half, sorted_right_half,
                                     descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || head->next == head || head->next->next == head) {
        return;
    }

    struct list_head *first_element = head->next;
    struct list_head *last_element = head->prev;

    // Break the circularity to form a linear list:
    // NULL <- first_element <-> ... <-> last_element -> NULL
    last_element->next = NULL;
    first_element->prev = NULL;

    struct list_head *sorted_list_first_element =
        sort_linear_list_recursive(first_element, descend);

    if (!sorted_list_first_element) {
        head->next = head;
        head->prev = head;
        return;
    }

    head->next = sorted_list_first_element;
    sorted_list_first_element->prev = head;

    // Find the tail of the sorted linear list and connect to head
    struct list_head *sorted_list_last_element = sorted_list_first_element;
    while (sorted_list_last_element->next != NULL) {
        sorted_list_last_element = sorted_list_last_element->next;
    }
    head->prev = sorted_list_last_element;
    sorted_list_last_element->next = head;
}

struct list_head *traversal_back_maintain_ascend_descend(struct list_head *head,
                                                         bool descend)
{
    if (!head || head->next == head || head->next->next == head)
        return head;

    struct list_head *next_node = head->prev, *curr_node = head->prev->prev;
    const char *curr_max_min = list_entry(next_node, element_t, list)->value;
    while (curr_node != head) {
        element_t *curr_entry = list_entry(curr_node, element_t, list);
        char *curr_value = curr_entry->value;
        struct list_head *curr_prev = curr_node->prev;
        if ((strcmp(curr_value, curr_max_min) <= 0 && !descend) ||
            (strcmp(curr_value, curr_max_min) >= 0 && descend)) {
            // maintain descend or ascend, move to next node
            curr_max_min = curr_value;
        } else {
            // delete curr_node
            list_del(curr_node);
            if (curr_entry->value) {
                free(curr_entry->value);
            }
            free(curr_entry);
        }
        curr_node = curr_prev;
    }
    return head;
}

/* Delete every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    head = traversal_back_maintain_ascend_descend(head, false);
    return q_size(head);
}

/* Delete every node which has a node with a strictly greater value anywhere
 * to the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    head = traversal_back_maintain_ascend_descend(head, true);
    return q_size(head);
}

int q_merge(struct list_head *meta_head, bool descend)
{
    if (!meta_head || list_empty(meta_head)) {
        return 0;
    }

    struct list_head *first_q_ctx_node = meta_head->next;
    queue_contex_t *first_q_ctx =
        list_entry(first_q_ctx_node, queue_contex_t, chain);
    struct list_head *result_q_sentinel = first_q_ctx->q;

    if (list_is_singular(meta_head)) {
        return q_size(result_q_sentinel);
    }
    // Temporary list to build the merged result
    LIST_HEAD(temp_merged_list_sentinel);

    while (true) {
        struct list_head *best_element_to_move = NULL;
        // No need to store origin_q_of_best_node if list_move_tail takes the
        // element directly

        struct list_head *ctx_iter_node;
        list_for_each(ctx_iter_node, meta_head) {
            queue_contex_t *current_ctx =
                list_entry(ctx_iter_node, queue_contex_t, chain);
            struct list_head *current_data_q = current_ctx->q;

            if (!list_empty(current_data_q)) {
                struct list_head *candidate_node =
                    current_data_q->next;  // First element
                const element_t *candidate_entry =
                    list_entry(candidate_node, element_t, list);

                if (best_element_to_move == NULL) {
                    best_element_to_move = candidate_node;
                } else {
                    const element_t *best_entry_so_far =
                        list_entry(best_element_to_move, element_t, list);
                    int cmp = strcmp(candidate_entry->value,
                                     best_entry_so_far->value);
                    bool take_candidate = false;
                    if (descend) {  // Larger values first
                        if (cmp > 0)
                            take_candidate = true;
                        // Optional: if cmp == 0, could add tie-breaking logic
                        // (e.g., by original queue ID) For now, simple
                        // comparison, first one wins on equality
                    } else {  // Smaller values first
                        if (cmp < 0)
                            take_candidate = true;
                    }
                    if (take_candidate) {
                        best_element_to_move = candidate_node;
                    }
                }
            }
        }

        if (best_element_to_move == NULL) {
            break;  // All source queues are now empty
        }

        // Move the chosen best_element_to_move to the tail of our temporary
        // merged list
        list_move_tail(best_element_to_move, &temp_merged_list_sentinel);
    }

    // All elements are now in temp_merged_list_sentinel, sorted.
    // Clear out the original destination queue (first queue in meta_head)
    // and splice the contents of temp_merged_list_sentinel into it.
    // Note: elements from the first queue were also moved into
    // temp_merged_list_sentinel.
    INIT_LIST_HEAD(result_q_sentinel);
    list_splice_init(&temp_merged_list_sentinel, result_q_sentinel);

    // Update sizes of all queues in the meta_list
    struct list_head *ctx_iter_node_for_size_update;
    list_for_each(ctx_iter_node_for_size_update, meta_head) {
        queue_contex_t *ctx =
            list_entry(ctx_iter_node_for_size_update, queue_contex_t, chain);
        if (ctx->q == result_q_sentinel) {
            ctx->size = q_size(result_q_sentinel);
        } else {
            // Other queues should now be empty because list_move_tail took
            // their elements
            ctx->size = 0;
            // Could also do INIT_LIST_HEAD(ctx->q); if not already guaranteed
            // by list_move_tail's list_del
        }
    }

    return q_size(result_q_sentinel);
}