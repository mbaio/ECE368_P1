/* This file contains the source code for ECE 368 Project #1 on priority queues

  Written by: Michael Baio and Igal Fleggman
  
  */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct linky {
  int actual_time;
  int service;
  int priority;
  struct linky * front;
  struct linky * back;
  struct linky * next;
  int time_in_queue;
}input_list;

input_list * List_merge (input_list *, input_list *);
input_list * generate_input1(float,float,float,int);
input_list * generate_input2(FILE *);
input_list * create_node(int,int,int);




input_list * create_node(int actual_time, int service, int priority)
{
  input_list * node = malloc(sizeof(input_list));
  node -> actual_time = actual_time;
  node-> service = service;
  node -> priority = priority;
  node -> time_in_queue = 0;
  node -> front = NULL;
  node -> back = NULL;
  node -> next = NULL;
  return node;
}

input_list * generate_input1(float lamba_0,float lamba_1,float mu,int total_tasks)
{
   int ind = 0;
   int current_time = 0;
   int intermediate_time = 0;
   int service = 0;
   int priority = 0;
   input_list * input_merged;
   srand(time(NULL));
   intermediate_time = (int)(ceil(-1 / lamba_0 * log(1-((double)rand() / (double)RAND_MAX)))); 
   service = (int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
   input_list * input0 = create_node(intermediate_time, service, priority);
   input_list * current = input0;
   current_time += intermediate_time;
   ind++;
      
   while (ind < total_tasks)
   {
     //srand(time(NULL));
     intermediate_time = (int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX))));
     service = (int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
     current_time += intermediate_time;
     current -> next = create_node(current_time,service,priority);
     current = current -> next;
    ind++; 
   }
   ind = 0;
   current_time = 0;
   priority = 1;
   //srand(time(NULL));
   intermediate_time = (int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX)))); 
   service = (int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
   input_list * input1 = create_node(intermediate_time, service, priority);
   current = input1;
   current_time += intermediate_time;
   ind++;
      
   while (ind < total_tasks)
   {
      //srand(time(NULL));
     intermediate_time = (int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX)))); 
     service = (int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 

     current_time += intermediate_time;
     current -> next = create_node(current_time,service,priority);
     current = current -> next;
    ind++; 
   }
   input_merged = List_merge(input1, input0);
   return input_merged;
}




input_list * List_merge (input_list * priority_one, input_list * priority_zero)
{
  //List * head;
  input_list * complete;
  //List *  tmp;
  //int first = 0; 

  if (priority_one == NULL)
    return priority_zero;
  else if (priority_zero == NULL)
    return priority_one;


if(priority_one -> actual_time > priority_zero -> actual_time) //compare if right is smaller than left
  {
    complete = priority_zero;
    complete -> next = List_merge (priority_one, priority_zero -> next);
  }
 else
   {
     complete = priority_one;
     complete -> next = List_merge (priority_zero, priority_one -> next);
   }
 return complete;
}

int compar (const void * a, const void * b)
{
    const int * aptr = (const int *) a; 
    const int * bptr = (const int *) b;
    return *aptr - *bptr; 
}


int main(int argc, char ** argv)
{
  //Generate mode
  printf("%d\n",argc);
  if (argc != 2 && argc != 5)
  {
    printf("\nNot valid option\n");
    return EXIT_FAILURE;
  }
  
  input_list * input;
  if (input == NULL)
    return EXIT_FAILURE;
  
  if (argc == 2)
  {
    FILE * fptr = fopen(argv[1],"r");
    if (fptr == NULL)
      return EXIT_FAILURE;
    //input = generate_input2(fptr);
  }
  else
  {
    float lamba_0 = atof(argv[1]);
    float lamba_1 = atof(argv[2]);
    float mu = atoll(argv[3]);
    int total_tasks = atoi(argv[4]);
    input = generate_input1(lamba_0,lamba_1,mu,total_tasks);
  }
  
  while (input != NULL)
  {
    printf("Actual time: %d Service time: %d Priority %d\n",input->actual_time,input->service,input->priority);
    input = input -> next;
  }
  
  return 0;
}

