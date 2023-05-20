/* A C file that includes realization of the functions used in
   Static Re-Reference Interval Prediction(RRIP) Cache Replacement */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "functions.h"
#include "./hash/ht-functions.h"
#include "./lru/lru.h"

int main()
{
	//FIXME:temporary code 
	Node_t **hash_RRIP;
	QNode **hash_LRU;
	long hashsize;
	//

	long cache_size, num_req, page, count_RRIP, count_LRU;
	/* 
	   num_req is an overall number of requests
	   page is a current request
	   count_RRIP tracks a number of cache hits in RRIP replacement
	   count_LRU tracks a number of cache hits in LRU replacement
	 */
	List_t *list;
	List_t *list2;
	Queue *queue;
	HashTable *table = create_table();

	//scanning
	while (!scanf("%ld", &cache_size) || (cache_size <= 0)) {
		printf
			("Waiting for the cache size... It must be positive number\n");
	}

	while (!scanf("%ld", &num_req) || (num_req <= 0)) {
		printf
			("Waiting for the overall number of requests... It must be positive number\n");
	}


	//FIXME: temporary code
	hash_RRIP = calloc(num_req + 1, sizeof(Node_t *));
	hash_LRU = calloc(num_req + 1, sizeof(QNode *));

	if ((hash_RRIP == NULL) || (hash_LRU == NULL)) {
		fprintf(stderr, "Memory exhausted\n");
		abort();
	}
	hashsize = num_req + 1;
	//

	list = create_list(cache_size);
	list2 = create_list(cache_size);
	queue = createQueue(cache_size);
	count_LRU = 0;
	count_RRIP = 0;

	//work begins...
	for (long i = 0; i < num_req; i++) {
		while (!scanf("%ld", &page) || (page <= 0)) {
			printf
				("Waiting for your request... It must be positive number\n");
		}

		//FIXME: temporarily we need to update hash each time
		if (hashsize - 1 < page) {
			hash_RRIP =
				(Node_t **) realloc(hash_RRIP,
						(page + 1) * sizeof(Node_t *));
			for (long i = hashsize; i <= page; i++)
				hash_RRIP[i] = NULL;

			hash_LRU =
				(QNode **) realloc(hash_LRU, (page + 1) * sizeof(QNode *));
			for (long i = hashsize; i <= page; i++)
				hash_LRU[i] = NULL;

			if ((hash_RRIP == NULL) || (hash_LRU == NULL)) {
				fprintf(stderr, "Memory exhausted(during realloc)\n");
				abort();
			}

			hashsize = page + 1;
		}
		//

		//count_LRU += lru(page, queue, hash_LRU);
		count_LRU += replacement_RRIP1(page, list2, hash_RRIP);
		count_RRIP += replacement_RRIP(page, list, table);
	}

	assert((count_LRU >= 0) && (count_RRIP >= 0)
			&& "Something went wrong, code doesn't work correctly");
	printf("Number of cache hits:\nfor RRIP %ld\nfor LRU %ld\n",
			count_RRIP, count_LRU);

	if ((count_LRU > count_RRIP) && (count_LRU != 0))
		printf("Suprisingly LRU has performed better...\n");

	if ((count_LRU == 0) && (count_RRIP == 0))
		printf("OMG, no cache hits at all...");


	delete_list(list);
	//delete_queue(queue);

	//free(hash) is needed
	return 0;
}
