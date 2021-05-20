#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entry {
	char* page;
	int frame;
	struct entry *next;
}
entry;


typedef struct mem {
	struct mem *previous;
	char* page;
	int frame;
	int written;
	int A;
	struct mem *next;
}
mem;

extern char **memory;
extern mem *memlinked;
extern int nframes;
extern entry **bztable;
extern entry **gctable;
extern int pagefault;
extern int *LRU;
extern int write;
extern int read;

void load(int key, char* lpage, int table, char action, char* algorithm);
int check(int key, char* lpage, int table);
int evict(mem** ret, char* algorithm);
void mount(entry* ptr,int f,mem* ret);
int hash(char* page);
void delete_entry( entry **head, char* lpage, int m);
void append_entry(entry** head, char* lpage, char action, char* algorithm);
void print(entry *node);
