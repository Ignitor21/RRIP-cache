/* A C file that includes realization of the functions used in
   Static Re-Reference Interval Prediction(RRIP) Cache Replacement */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "functions.h"
#include "./hash/ht-linked-list.h"
#include "./hash/ht-functions.h"


List_t* create_list(long size)
{
        List_t* list = calloc(1, sizeof(List_t));
        list->head = list->fst_dist = list->tail = NULL;
        list->size = size;
        list->full_nodes = 0;

        assert(list != NULL && "Heap overflow!");

        return list;
}

Node_t* newNode(long data) 
{
        Node_t* res = calloc(1, sizeof(Node_t));
        res->next = res->prev = NULL;
        res->data = data;
        res->value = 2;
        
        return res;
}

int isListFull(List_t* list)
{
    return list->size == list->full_nodes;
}

int isListEmpty(List_t* list)
{
        return list->tail == NULL;
}


void dequeue(Node_t* node, List_t* list, HashTable* table) 
{
        list->fst_dist = node->next;

        if (node != list->head) 
                node->prev->next = node->next;
        else 
                list->head = node->next;

        if (node != list->tail)
                node->next->prev = node->prev;
        else 
                list->tail = node->prev;
        
        
        list->full_nodes--;
        ht_delete(table, node->data);
        //hash[node->data] = NULL;
        free(node);
}

void enqueue(List_t* list, HashTable* table, long data)
{
        Node_t* curnode = newNode(data);

        if (isListFull(list)) {
                if (list->fst_dist == NULL) {
                        int i = 3 - list->tail->value; 
                        Node_t* tmp = list->head;
                        
                        for (;tmp != NULL;) {
                                tmp->value += i;
                                if ((tmp->value == 3) && (list->fst_dist == NULL)) {
                                        list->fst_dist = tmp;
                                }
                                tmp = tmp->next;
                        }
                }

                Node_t* hlp = list->fst_dist->prev;
                dequeue(list->fst_dist, list, table);
                
                if (hlp != NULL) {
                        if (hlp != list->tail)
                                hlp->next->prev = curnode;
                        else 
                                list->tail = curnode;
                        hlp->next = curnode;   
                } else {
                        list->head->prev = curnode;
                        list->head = curnode;
                }

                curnode->next = list->fst_dist;
                curnode->prev = hlp;

                list->full_nodes++;
                ht_insert(table, data, curnode);
                //hash[data] = curnode;
        } else {
                if (isListEmpty(list)) {
                        list->head = curnode;
                } else {
                        list->tail->next = curnode;
                        curnode->prev = list->tail;
                }
                list->tail = curnode;
                list->full_nodes++;
                ht_insert(table, data, curnode);
                //hash[data] = curnode;
        }

}

void cache_hit(Node_t* node, List_t* list)
{
        if (list->head == node)
        {
                list->head->value = 0;
                return;
        }

        if (list->tail == node)
        {
                list->tail = node->prev;
                list->tail->next = NULL;
                list->head->prev = node;
                node->next = list->head;
                node->prev = NULL; 
                list->head = node;
                node->value = 0;
                return;
        }

        if (list->fst_dist == node)
                list->fst_dist = node->next;           

        node->next->prev = node->prev;
        node->prev->next = node->next;
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;  
        node->value = 0;

        return;
}


int replacement_RRIP(long page, List_t* list, HashTable* table)
{       
        Node_t* node = NULL;

        if (node = ht_search(table, page)) // Change arguments
        {
                cache_hit(node, list);
                return 1;
        }

        else
        {
                enqueue(list, table, page);
                return 0;
        }
}


//FIXME
/*int replacement_RRIP(long page, List_t* list, Node_t** hash)
{       
        Node_t* node = NULL;

        if (node = is_element_in_ht(page)) // Change arguments
        {
                cache_hit(node, list);
                return 1;
        }

        else
        {
                enqueue(list, hash, page);
                return 0;
        }
}*/

void print_list(List_t* list)
{
        Node_t* head = list->head;

        while (head != NULL)
        {
                printf("%ld ", head->data);
                head = head->next;
        }

        printf("\n");
        return;
}

void delete_list(List_t* list)
{
        Node_t* next;
        Node_t* top = list->head;

	while (top != NULL)
	{
		next = top->next;
		free(top);
		top = next;
	}

        free(list);
}

void dequeue1(Node_t* node, List_t* list, Node_t ** hash) 
{
        list->fst_dist = node->next;

        if (node != list->head) 
                node->prev->next = node->next;
        else 
                list->head = node->next;

        if (node != list->tail)
                node->next->prev = node->prev;
        else 
                list->tail = node->prev;
        
        
        list->full_nodes--;
        hash[node->data] = NULL;
        free(node);
}

void enqueue1(List_t* list, Node_t ** hash, long data)
{
        Node_t* curnode = newNode(data);

        if (isListFull(list)) {
                if (list->fst_dist == NULL) {
                        int i = 3 - list->tail->value; 
                        Node_t* tmp = list->head;
                        
                        for (;tmp != NULL;) {
                                tmp->value += i;
                                if ((tmp->value == 3) && (list->fst_dist == NULL)) {
                                        list->fst_dist = tmp;
                                }
                                tmp = tmp->next;
                        }
                }

                Node_t* hlp = list->fst_dist->prev;
                dequeue1(list->fst_dist, list, hash);
                
                if (hlp != NULL) {
                        if (hlp != list->tail)
                                hlp->next->prev = curnode;
                        else 
                                list->tail = curnode;
                        hlp->next = curnode;   
                } else {
                        list->head->prev = curnode;
                        list->head = curnode;
                }

                curnode->next = list->fst_dist;
                curnode->prev = hlp;

                list->full_nodes++;
                hash[data] = curnode;
        } else {
                if (isListEmpty(list)) {
                        list->head = curnode;
                } else {
                        list->tail->next = curnode;
                        curnode->prev = list->tail;
                }
                list->tail = curnode;
                list->full_nodes++;
                hash[data] = curnode;
        }

}

int replacement_RRIP1(long page, List_t* list,  Node_t ** hash)
{       
        Node_t* node = NULL;

        if (node = hash[page]) // Change arguments
        {
                cache_hit(node, list);
                return 1;
        }

        else
        {
                enqueue1(list, hash, page);
                return 0;
        }
}

void update_hash(long* phashsize, long page, Node_t** hash_RRIP, QNode** hash_LRU) {
        hash_RRIP = (Node_t **) realloc(hash_RRIP, (page + 1) * sizeof(Node_t *));
        for (long i = *phashsize; i <= page; i++)
                hash_RRIP[i] = NULL;

        hash_LRU = (QNode **) realloc(hash_LRU, (page + 1) * sizeof(QNode* ));
        for (long i = *phashsize; i <= page; i++)
                hash_LRU[i] = NULL;

        if ((hash_RRIP == NULL) || (hash_LRU == NULL)) {
                fprintf(stderr, "Memory exhausted(during realloc)\n");
                abort();
        }   

        *phashsize = page + 1;
}