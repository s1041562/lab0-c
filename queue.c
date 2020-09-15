#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    while (q->head) {
        list_ele_t *temp = q->head;
        q->head = q->head->next;
        free(temp->value);
        free(temp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!q || !newh) {
        return false;
    }
    int length = strlen(s) + 1;
    newh->value = malloc(length * sizeof(char));
    if (!newh->value) {
        /*Avoid memory leak.*/
        free(newh);
        return false;
    }
    memset(newh->value, '\0', length);
    strncpy(newh->value, s, strlen(s));
    newh->next = q->head;
    q->head = newh;
    /* Check newh is the only element*/
    if (!q->tail) {
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!q || !newh) {
        return false;
    }
    int length = strlen(s) + 1;
    newh->value = malloc(length * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memset(newh->value, '\0', length);
    strncpy(newh->value, s, strlen(s));
    q->tail->next = newh;
    q->tail = newh;
    if (!q->head) {
        q->head = newh;
    }
    newh->next = NULL;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }
    list_ele_t *temp;
    temp = q->head;
    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, temp->value, bufsize - 1);
    }
    q->head = q->head->next;
    free(temp->value);
    free(temp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }
    list_ele_t *temp;
    q->tail->next = q->head;
    temp = q->head;
    q->head = q->head->next;
    while (q->head != q->tail) {
        q->tail->next->next = q->head;
        q->head = q->head->next;
        q->tail->next->next->next = temp;
        temp = q->tail->next->next;
    }
    q->tail = q->tail->next;
    q->head->next = temp;
    q->tail->next = NULL;
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }
    q->head = merge_sort_list(q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
    return;
}

list_ele_t *merge_sort_list(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    /* spilt list */
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = merge_sort_list(head);
    list_ele_t *l2 = merge_sort_list(fast);
    return merge(l1, l2);
}

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t q;
    list_ele_t *temp = &q;
    size_t len = (strlen(l1->value) < strlen(l2->value)) ? strlen(l1->value)
                                                         : strlen(l2->value);
    while (l1 && l2) {
        if (strncmp(l1->value, l2->value, len) < 0) {
            temp->next = l1;
            temp = temp->next;
            l1 = l1->next;
        } else {
            temp->next = l2;
            temp = temp->next;
            l2 = l2->next;
        }
    }
    if (l1) {
        temp->next = l1;
    }
    if (l2) {
        temp->next = l2;
    }
    return q.next;
}
