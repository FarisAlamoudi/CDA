#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CS 32
#define bs 4

// Helper function to access the cache
typedef struct cache {
    int size;   
    int as;  
    int ns;   
    int bs; 
    int recd;    
    int hit;    
    int miss;   
    int **tag;  
    int **valid;    
    char **data;    
    int *lru;   
} Cache;

void access_cache(Cache *cache, int addr) {
    int set_index = (addr / cache->bs) % cache->ns;
    int tag = (addr / cache->bs) / cache->as;
    int i, j, lru_index, random_index;
    for (i = 0; i < cache->as; i++) {
        if (cache->valid[set_index][i] && cache->tag[set_index][i] == tag) {
            cache->hit++;
            if (cache->recd == 0) {
                cache->lru[set_index] = i;
                return;
}
}
}
cache->miss++;
if (cache->recd == 0) {
lru_index = cache->lru[set_index];
for (i = 0; i < cache->as; i++) {
if (!cache->valid[set_index][i]) {
lru_index = i;
break;
}
if (!cache->valid[set_index][i] || cache->lru[set_index] > cache->lru[set_index + i]) {
lru_index = i;
}
}
cache->lru[set_index + lru_index] = cache->lru[set_index] + 1;
if (lru_index == cache->as) {
lru_index = 0;
cache->lru[set_index] = 0;
}
cache->tag[set_index][lru_index] = tag;
cache->valid[set_index][lru_index] = 1;
memcpy(&cache->data[set_index][lru_index * cache->bs], (void *) addr, cache->bs * sizeof(char));
} else {
random_index = rand() % cache->as;
cache->tag[set_index][random_index] = tag;
cache->valid[set_index][random_index] = 1;
memcpy(&cache->data[set_index][random_index * cache->bs], &addr, cache->bs * sizeof(char));
}
}
// Helper function to convert hexadecimal strings to integers
int hex_to_int(char *hex) {
    return (int) strtol(hex, NULL, 16);
}

// Helper function to initialize the cache
void init_cache(Cache *cache, int as, int recd) {
    int i, j;
    int size;
    cache->as = as;
    cache->ns = cache->size / (as * cache->bs);
    cache->recd = recd;
    cache->hit = 0;
    cache->miss = 0;
    cache->tag = (int **) malloc(cache->ns * sizeof(int *));
    cache->valid = (int **) malloc(cache->ns * sizeof(int *));
    cache->data = (char **) malloc(cache->ns * sizeof(char *));
    if (recd == 0) {
        cache->lru = (int *) malloc(cache->ns * sizeof(int));
        memset(cache->lru, 0, cache->ns * sizeof(int));
    }
    for (i = 0; i < cache->ns; i++) {
        cache->tag[i] = (int *) malloc(as * sizeof(int));
        cache->valid[i] = (int *) malloc(as * sizeof(int));
        cache->data[i] = (char *) malloc(cache->bs * as * sizeof(char));
        for (j = 0; j < as; j++) {
            cache->tag[i][j] = -1;
            cache->valid[i][j] = 0;
            memset(&cache->data[i][j * cache->bs], 0, cache->bs * sizeof(char));
        }
    }
}


int main() {
Cache cache;
char hex[9];
int addr;
int i;
// Initialize cache with size 32, asiativity 1, block size 4, and replacement policy LRU
cache.size = CS;
cache.bs = bs;
init_cache(&cache, 1, 0);
// Access addresses in a loop
for (i = 0; i < 16; i++) {
    // Read hexadecimal address from input
    scanf("%s", hex);
    // Convert hexadecimal address to integer
    addr = hex_to_int(hex);
    // Access the cache using the address
    access_cache(&cache, addr);
}

// Print cache statistics
printf("Hits: %d\n", cache.hit);
printf("Misses: %d\n", cache.miss);

return 0;
}
