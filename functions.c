#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


int choose(char* lpage)
{
    int i=0;
    mem* ptr;
    mem* tail;
    mem* prev;
    int j=0;
    while(i<nframes)
    {
        if(memory[i]!=NULL){
            i++;
            continue;
        }
        LRU[i]=0;
        memory[i]=malloc(5+1);
        strcpy(memory[i],lpage);
        ptr = malloc(sizeof(mem));
        //if (ptr == NULL)
        //    return;
        ptr->page=malloc(11+1);
        strcpy(ptr->page,lpage);
        ptr->frame=i;
        ptr->previous=NULL;
        ptr->next=NULL;
        ptr->A=0;
        if (i==0)
            memlinked=ptr;
        else{
            tail=memlinked;
            prev = tail;
            j=i;
            while(j)
            {
                prev = tail;
                tail = tail->next;
                j--;
            }
            ptr->previous=prev;
            prev->next=ptr;
            if(i==nframes-1)
                ptr->next=memlinked;
        }
        mem* p;
        j=0;
        for (p = memlinked; p && j<10; p = p->next){
          //printf("mem page: %s in frame: %d\n",p->page,p->frame);
            j++;
        }
        return i;
    }
    return nframes+1;
}

int hash(char* page)
{
    int x=(int)page[0]-'0';
    //int y=(int)page[1]-'0';
    //int z=(int)page[2]-'0';
    //int a=(int)page[3]-'0';
    //int b=(int)page[4]-'0';
    //return  x+y/2+z/3+a/4+b/5;
    return x;
}

void load(int key, char* lpage, int table, char action, char* algorithm)
{
    //printf("check:%d");
    //if(action=='W')
    //    printf("%c",action);

    //printf(" c is: %d ",c);
    if(check(key, lpage, table)==0)
    {
        pagefault++;
        read++;
        //printf("  pagefault\n");
        int new;
        int b=0;
        entry* tail;
        entry* ptr = malloc(sizeof(entry));

        if (ptr == NULL)
            return;
        if (table==0)
            append_entry(&bztable[key], lpage, action, algorithm);
        else
            append_entry(&gctable[key], lpage, action, algorithm);
    }
}

int check(int key, char* lpage, int table)
{
    int found=0;
    entry* tail;
    mem* find=NULL;
    int j=0;
    int i=0;
    if (table==0)
        tail = bztable[key];
    else
        tail = gctable[key];
    if(tail!=NULL){
      //printf("\nmycheck %s\n",tail->page);

        //printf("\nCheck");
        while (tail!=NULL)
        {
            //printf(" page %s in frame %d, ",tail->page,tail->frame);
            if(tail==NULL)
                break;
            if (strcmp(tail->page,lpage)==0){
//                printf("\nCHECKED: page: %s or %s in frame: %d, iosto:%d\n",lpage,tail->page,tail->frame,i);
                found=1;
                find=memlinked;
                while((strcmp(find->page,lpage)!=0)&&(j<nframes)){
                    //printf("check: page: %s\n",find->page);
                    find=find->next;
                    j++;
                }
                find->A=1;
                LRU[tail->frame]++;
                //printf("\npage:%s findA: %d\n",find->page,find->A);
                break;
            }
            else{
                tail = tail->next;
                i++;}
        }
    }
    else{
        //printf("\ntail is None\n");
    }
    return found;
}

int evict(mem** ret, char* algorithm)
{
    int m;
    int key;
    mem* clock=memlinked;
    //printf("ret: %s",ret);
    //that is second chance
    if(strcmp(algorithm,"second_chance")==0)
        while(1){
            //if(j<nframes){
    //        printf("\nevict: page:%s a:%d\n",clock->page,clock->A);
            if(clock->A==1){
                clock->A=0;
                clock=clock->next;
            }
            else{
                m=clock->frame;
                *ret=clock;
                break;
            }
            //j++;
        }
    //that is lru algorithm
    else if(strcmp(algorithm,"lru")==0){
        int min=10000;
        //printf("\n");
        //for(int i=0;i<nframes;i++)
        //		printf("LRU[%d]=%d  ",i,LRU[i]);
        for(int i=0;i<nframes;i++)
        {
            if(LRU[i]<=min){
                min=LRU[i];
                m=i;
            }
        }
        while(1){
            //if(j<nframes){
    //        printf("\nevict: page:%s a:%d\n",clock->page,clock->A);
            if(clock->frame!=m){
                clock=clock->next;
            }
            else{
                *ret=clock;
                break;
            }
            //j++;
        }
    }
    if (clock->written==1)
        write++;
    //printf("\npage evicted: %s from frame: %d\n",clock->page,m);
    key=hash(memory[m]);
    delete_entry(&gctable[key],memory[m],m);
    delete_entry(&bztable[key],memory[m],m);

    free(memory[m]);
    memory[m]=NULL;
    return m;
}

void mount(entry* ptr,int f,mem* ret)
{
    LRU[f]=0;
    ptr->frame=f;
    memory[f]=malloc(5+1);
    strcpy(memory[f],ptr->page);
    strcpy(ret->page,ptr->page);
    ret->written=0;
    ret->A=0;

}

void delete_entry(entry **head, char* lpage, int m)
{
    entry* temp = *head, *prev;
    if((temp!=NULL)&&(strcmp(temp->page,lpage)==0)&&(temp->frame==m))
    {
        *head = temp->next;
        free(temp);
        return;
    }
    while(temp!=NULL)
    {
        if(strcmp(temp->page,lpage)==0&&(temp->frame==m))
            break;
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}

void append_entry(entry** head, char* lpage, char action, char* algorithm)
{
    entry* ptr =malloc(sizeof(entry));
    ptr->page=malloc(11+1);
    entry *last = *head;
    strcpy(ptr->page,lpage);
    ptr->frame=choose(lpage);
    if (ptr->frame==nframes+1){
      mem* pointer=NULL;
      int new=evict(&pointer,algorithm);
      mount(ptr, new, pointer);
    }
    if(action=='W'){
        mem* w=memlinked;
        while(1){
            if(w->frame!=ptr->frame){
                w=w->next;
            }
            else{
                w->written=1;
                break;
            }
        }
    }
    ptr->next = NULL;
    if (*head == NULL)
    {
       *head = ptr;
       return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = ptr;
    return;
}


void print(entry *node)
{
    if (node!=NULL)
        printf("\n\nprinting table[%d]\n",hash(node->page));
    while (node != NULL)
    {
        printf(" %s \n", node->page);
        node = node->next;
    }
}
