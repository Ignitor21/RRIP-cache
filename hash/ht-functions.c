#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../functions.h"
#include "ht-linked-list.h"
#include "ht-functions.h"


NodeHtLl** create_overflow_list(HashTable* table) {
    NodeHtLl** buckets = (NodeHtLl**) calloc (modular, sizeof(NodeHtLl*));
    for (int i=0; i < modular; ++i)
        buckets[i] = NULL;
    return buckets;
}

void free_overflow_list(HashTable* table) {
    NodeHtLl** buckets = table->overflow_list;
    for (int i=0; i < modular; ++i)
        ht_ll_free(buckets[i]);
    free(buckets);
}


HtElem* create_elem(long data, Node_t * value) {
    HtElem* elem = (HtElem*) malloc (sizeof(HtElem));
    
	elem->data = data;
	elem->value = value;

    return elem;
}

HashTable* create_table() {
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->elems = (HtElem**) calloc (modular, sizeof(HtElem*));
    for (int i=0; i<modular; ++i)
        table->elems[i] = NULL;
    table->overflow_list = create_overflow_list(table);

    return table;
}

void free_table(HashTable* table) {
    for (int i=0; i<modular; ++i) {
        HtElem* elem = table->elems[i];
        if (elem != NULL)
            free(elem);
    }

    free_overflow_list(table);
    free(table->elems);
    free(table);
}

void solve_collision(long index, HtElem* elem, HashTable* table) {
    NodeHtLl* head = table->overflow_list[index];

    if (head == NULL) {
        head = ht_ll_create();
        head->elem = elem;
        table->overflow_list[index] = head;
        return;
    }
    else {
        //table->overflow_list[index] = linkedlist_insert(head, elem);
        //table->overflow_list[index] = ht_ll_insert(head, elem);       choose right option please
        return;
    }
 }

void ht_insert(HashTable* table, long data, Node_t * value) {
    HtElem* elem = create_elem(data, value);

    unsigned long index = hash_function(data);

    HtElem* current_elem = table->elems[index]; 
    
    if (current_elem == NULL) {
        table->elems[index] = elem; 
    }

    else {
            if (current_elem->data == data) {
                table->elems[index]->value = value;
				value = table->elems[index]->value;
                return;
            }
    
        else {
            solve_collision(index, elem, table);
            return;
        }
    }
}

Node_t* ht_search(HashTable* table, long data) {
    int index = hash_function(data);
    HtElem* elem = table->elems[index];
    NodeHtLl* head = table->overflow_list[index];

    while (elem != NULL) {
        if (elem->data == data)
            return elem->value;
        if (head == NULL)
            return NULL;
        elem = head->elem;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, long data) {
    // Deletes an elem from the table
    int index = hash_function(data);
    HtElem* elem = table->elems[index];
    NodeHtLl* head = table->overflow_list[index];

    if (elem == NULL) {
        // Does not exist. Return
        return;
    }
    else {
        if (head == NULL && elem->data == data) {
            // No collision chain. Remove the elem
            // and set table index to NULL
            table->elems[index] = NULL;
            free(elem);
            return;
        }
        else if (head != NULL) {
            // Collision Chain exists
            if (elem->data == data) {
                // Remove this elem and set the head of the list
                // as the new elem
                
                free(elem);
                NodeHtLl* node = head;
                head = head->next;
                node->next = NULL;
                table->elems[index] = create_elem(node->elem->data, node->elem->value);
                ht_ll_free(node);
                table->overflow_list[index] = head;
                return;
            }

            NodeHtLl* curr = head;
            NodeHtLl* prev = NULL;
            
            while (curr) {
                if (curr->elem->data == data) {
                    if (prev == NULL) {
                        // First element of the chain. Remove the chain
                        ht_ll_free(head);
                        table->overflow_list[index] = NULL;
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        ht_ll_free(curr);
                        table->overflow_list[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }

        }
    }
}