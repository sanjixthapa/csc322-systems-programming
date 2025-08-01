
// when compiling use,  gcc main.c -lm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//line in the cache
typedef struct {
    bool valid;
    int tag;
    int hits; // LFU
    int lastAccess; // LRU
} CacheLine;

int main() {
    // input
    int S, E, B, m;
    int h, p;
    char policy[4];

    scanf("%d %d %d %d", &S, &E, &B, &m);
    scanf("%s", policy);
    scanf("%d %d", &h, &p);
    printf("\n");

    int offsetBits = log2(B); //offset bits = log2(block size)
    int indexBits = log2(S); //index bits = log2(number of sets)
    int tagBits = m - indexBits - offsetBits;

    CacheLine **cache = malloc(S * sizeof(CacheLine *));
    for (int i = 0; i < S; i++) {
        cache[i] = (CacheLine *)calloc(E, sizeof(CacheLine));
    }

    int totalCycles = 0;
    int numAddresses = 0;
    int numMisses = 0;
    unsigned int address;
    int tag = 0, setIndex = 0;

    //reading memory addresses
    while (scanf("%x", &address)) {
        if (address == -1) {
            break;
        }
        numAddresses++;

        //extract tag and set index from the address
        tag = address >> (offsetBits + indexBits);
        setIndex = (address >> offsetBits) & (S - 1);

        //look for a cache hit
        bool hit = false;
        for (int i = 0; i < E; i++) {
            if (cache[setIndex][i].valid && cache[setIndex][i].tag == tag) {
                //cache hit
                hit = true;
                cache[setIndex][i].hits++; //lfu
                cache[setIndex][i].lastAccess = numAddresses; //lru
                printf("%.2x H\n", address);
                break;
            }
        }
        //miss
        if (!hit) {
            numMisses++;
            printf("%.2x M\n", address);
            totalCycles = totalCycles + p; //miss penalty

            int replacementLine = 0;
            for (int i = 0; i < E; i++) {
                //empty line is found
                if (!cache[setIndex][i].valid) {
                    replacementLine = i;
                    break;
                }
                if (strcmp(policy, "LFU") == 0) {
                    if (cache[setIndex][i].hits < cache[setIndex][replacementLine].hits) {
                        replacementLine = i;
                    }
                }
                else if (strcmp(policy, "LRU") == 0) {
                    if (cache[setIndex][i].lastAccess < cache[setIndex][replacementLine].lastAccess) {
                        replacementLine = i;
                    }
                }
            }
            //replace the selected line in cache
            cache[setIndex][replacementLine].valid = true;
            cache[setIndex][replacementLine].tag = tag;
            cache[setIndex][replacementLine].hits = 1; // reset hits
            cache[setIndex][replacementLine].lastAccess = numAddresses; //current access time
        }
        totalCycles = totalCycles + h;
    }

    double missRate = (numMisses * 100.0) / numAddresses;
    printf("%.f %d\n", missRate, totalCycles);

    for (int i = 0; i < S; i++) {
        free(cache[i]);
    }
    free(cache);

    return 0;
}
