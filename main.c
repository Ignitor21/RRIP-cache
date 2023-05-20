/* A C file that includes realization of the functions used in
   Static Re-Reference Interval Prediction(RRIP) Cache Replacement */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "./lru/lru.h"
#include "functions.h"
#include "./hash/ht-functions.h"



int main()
{
	Node_t** hash_RRIP;
    QNode** hash_LRU;
    long hashsize; 
    HashTable *table;
    
    long cache_size, num_req, page, count_RRIP, count_LRU, count_check; 
    /* 
        num_req is an overall number of requests
        page is a current request
        count_RRIP tracks a number of cache hits in RRIP replacement
        count_LRU tracks a number of cache hits in LRU replacement
        count_check is 
    */
    List_t* list;
    List_t* list_check;
    Queue* queue;

    //scanning
    while(!scanf("%ld", &cache_size) || (cache_size <= 0)) 
        printf("Waiting for the cache size... It must be positive number\n");

    while(!scanf("%ld", &num_req) || (num_req <= 0)) 
        printf("Waiting for the overall number of requests... It must be positive number\n");
    
    hashsize = num_req + 1;
    hash_RRIP = calloc(hashsize, sizeof(Node_t *));
    hash_LRU = calloc(hashsize, sizeof(QNode *));
    table = create_table();
    list = create_list(cache_size);
    list_check = create_list(cache_size);
    queue = createQueue(cache_size);

    if ((hash_RRIP == NULL) || (hash_LRU == NULL) || (table == NULL) || (list == NULL) || (list_check == NULL) || (queue == NULL)) {
        fprintf(stderr, "Memory exhausted\n");
        abort();
    }   
  
    count_LRU = 0;
    count_RRIP = 0;
    count_check = 0;

    //work begins...
    for (long i = 0; i < num_req; i++) 
    {
        while(!scanf("%ld", &page) || (page <= 0)) {
            printf("Waiting for your request... It must be positive number\n");
        }   
   
        if (hashsize - 1 < page) {
            update_hash(&hashsize, page, hash_RRIP, hash_LRU);
        }
             
		count_LRU += lru(page, queue, hash_LRU);
        count_RRIP += replacement_RRIP(page, list, table);
        count_check += replacement_RRIP1(page, list_check, hash_RRIP);
    }

    assert((count_LRU >= 0) && (count_RRIP >= 0) && "Something went wrong, code doesn't work correctly");  
    printf("Number of cache hits:\nfor RRIP %ld (%ld)\nfor LRU %ld\n", count_RRIP, count_check, count_LRU);

	if ((count_LRU == 0) && (count_RRIP == 0))
		printf("OMG, no cache hits at all...");


	delete_list(list);

    delete_list(list_check);
    free_table(table);
    //delete_queue(queue);


	//free(hash) is needed
	return 0;
}
