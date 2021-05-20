#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


int nframes;
int pagefault=0;
char **memory;
int *LRU;
mem *memlinked;
entry **bztable;
entry **gctable;
int write=0;
int read=0;

int main(int argc, char *argv[])
{
	int inmem=0;
	int i=0;
	bztable=malloc(100 * sizeof(entry*));
	gctable=malloc(100 * sizeof(entry*));
	for(i=0;i<100;i++)
		bztable[i]=NULL;
	for(i=0;i<100;i++)
		gctable[i]=NULL;
	entry* p;
	memlinked=NULL;
	if(argc<4)
	{
        printf("Wrong parameters\n./project <replacementAlgorithm[1/2]> <memoryFrames> <q_traces> [<maxTraces>]\n");
		return 1;
	}
	int alg=atoi(argv[1]);
    char algorithm[15];
    nframes=atoi(argv[2]);
    int q=atoi(argv[3]);
    int max=1000000;
	int hashed;
	int counter=0;
	int j=0;
	char** bzip;
	char** gcc;
	if(alg==1)
		strcpy(algorithm,"lru");
	else if(alg==2)
		strcpy(algorithm,"second_chance");
	else
	{
		printf("Wrong replacement algorithm. Choose again [1/2]\n",algorithm);
		strcpy(algorithm,"second_chance");
	}
	printf("The current replacement algorithm is <%s>\nThe memory has %d frames\nThe program will alternately read %d traces from each file\n",algorithm,nframes,q);
	if(argc>4){
		max=atoi(argv[4]);
		printf("The number of traces to be examined from the traces' files is %d\n\n",max);
	}
	else
		printf("All traces will be examined from the traces' files. That is, %d traces.\n\n",max);


	memory = malloc(nframes * sizeof(char*));
	for(i=0;i<nframes;i++)
			memory[i]=NULL;
	LRU=malloc(nframes*sizeof(int));
	bzip = malloc(max * sizeof(char*));
	gcc = malloc(max * sizeof(char*));
    printf(" %s %d %d %d\n",algorithm,nframes,q,max,argc);
	char buf[12];
	char temp[9];
	FILE *file;

	i=0;
	file = fopen("bzip.trace", "r");
	if (file) {
		while (fgets(buf,12, file)&&i<max){
			bzip[i]= malloc(11+1);
			strcpy(bzip[i],buf);
			i++;
		}
		if (ferror(file)) {
			return 1;
		}
		fclose(file);
	}

	file = fopen("gcc.trace", "r");
	if (file) {
			i=0;
		while (fgets(buf,12, file)&&i<max){
			gcc[i]= malloc(11+1);
			strcpy(gcc[i],buf);
			i++;
		}
		if (ferror(file)) {
			return 1;
		}
		fclose(file);
	}

	while(counter<max)
	{
		for (i=0;i<q && counter+i<max;i++)
		{

			// hash the name into a spot
			int hashed = hash(bzip[counter+i]);
			inmem=0;
			// insert the name in table with hashed value
			strncpy(temp,bzip[counter+i],5);
			temp[6]='\0';
			//printf("\n\n\n%d) bzip: %s key:%d",i,temp,hashed);
			load(hashed, temp, 0, bzip[counter+i][9], algorithm);
		}
		for (i=0;i<q && counter+i<max;i++)
		{
			// hash the name into a spot
			int hashed = hash(gcc[counter+i]);
			inmem=0;
			// insert the name in table with hashed value
			strncpy(temp,gcc[counter+i],5);
			temp[6]='\0';
			load(hashed, temp, 1, gcc[counter+i][9], algorithm);
		}
		counter=counter+q;
		if (counter>=max)
			break;
	}

	printf("\n\nStatistics collected from the current execution:\n%d writes to disk, \n%d reads from disk, \n%d page faults, \n%d entries were examined from each of the traces' file \nmemory of %d frames\n",write,read,pagefault,max,nframes);


	for (i=0;i<max;i++)
	{
		free(bzip[i]);
		free(gcc[i]);
	}
	for(i=0;i<100;i++)
	{
		free(bztable[i]);
		free(gctable[i]);
	}
	for(i=0;i<nframes;i++)
		free(memory[i]);
	free(bzip);
	free(gcc);
	free(memory);
	free(bztable);
	free(gctable);
}
