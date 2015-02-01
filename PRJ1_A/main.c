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
  int time_out;
  struct linky * next;
  struct linky * buffnext;
  int time_in_queue;
}input_list;

typedef struct queue_length {
  int length;
  struct queue_length * next;
}queue_len;

typedef struct buffy {
  input_list * front;
  input_list * back;
}buffer_list;


input_list * List_merge (input_list *, input_list *);
input_list * generate_input1(float,float,float,int);
input_list * generate_input2(FILE *);
input_list * create_node(int,int,int);
void destroy_list (input_list *);
char * * explode(const char *, const char *, int *);



input_list * create_node(int actual_time, int service, int priority)
{
  input_list * node = malloc(sizeof(input_list));
  node -> actual_time = actual_time;
  node-> service = service;
  node -> priority = priority;
  node -> time_in_queue = 0;
  node -> time_out = 0;
  node -> next = NULL;
  node -> buffnext = NULL;
  return node;
}

input_list * generate_input2 (FILE * ptr)
{
  char str [60];
  int alength;
  char ** inputs;
  int arrival;
  int priority;
  int service;
  char delims = ' ';
  input_list * input0;
  input_list * current;
  if (fgets(str, 60, ptr) != NULL)
    {
      inputs  = explode (str, &delims, &alength);
      arrival = atoi (inputs[0]);
      priority = atoi (inputs[1]);
      service = atoi (inputs[2]);
      input0 = create_node(arrival, service, priority);
      current = input0;
      free(inputs[0]);
      free(inputs[1]);
      free(inputs[2]);
      free(inputs);
    }
  else
    {
      return NULL;
    }
  while (fgets(str, 60, ptr) != NULL)
    {
      inputs  = explode (str, &delims, &alength);
      arrival = atoi (inputs[0]);
      priority = atoi (inputs[1]);
      service = atoi (inputs[2]);
      printf (" arriva %d priority = %d service = %d\n", arrival, priority, service);
      current -> next = create_node(arrival, service, priority);
      current = current -> next;
      free(inputs[0]);
      free(inputs[1]);
      free(inputs[2]);
      free(inputs);
    }
  fclose(ptr);
  return input0;
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
void destroy_list (input_list * head){
  if (head  == NULL)
    return;
  destroy_list(head -> next);
  free (head);
  return;
}
  

char * * explode(const char * str, const char * delims, int * arrLen)
{// it mallocs for you
  // it gives you back arrLength
  int ind = 0; 
  int numdelim = 0;
  int letters = 0;
  const char * first; 
  int tempind = 0;
  while (str[ind] != '\0')
    {//count delimeters
      if ((strchr(delims,str[ind])) != NULL)
	{
	numdelim++; //add another delimeter to the count
	}
      ind++;
    }//end counting of delimeters
  char ** dest = malloc((numdelim + 1) * sizeof(char*));
  first = str;
  *arrLen = numdelim + 1;
  for(ind = 0; ind <= numdelim; ind ++){
     
    while ((strchr(delims,str[tempind])) == NULL)
	{
	  letters++;
	  tempind++;
	}
    dest[ind] = malloc((letters + 1) * sizeof(char));
    memcpy(dest[ind],first,(sizeof(char)*letters));
    dest[ind][letters] = '\0';
    tempind++;
    first +=  (sizeof(char)*(1 + letters));
    letters = 0;
  }
return dest;
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
    input = generate_input2(fptr);
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
  //declarations #billy crum
  buffer_list queue0_list;
  buffer_list queue1_list;
  input_list * time_ptr = input;
  int time = 0;
  int status = 0;
  int queue0 = 0;
  int queue1 = 0;
  int size_queue = 0;
  int cpu_usage = 0;
  int avg_waitng0 = 0;
  int avg_waitng1 = 0;
  int out = 0;
  input_list * server_ptr == NULL;
  while (out == 0)
    {
      if (status == 0)
	{
	  if (queue0 > 0)
	    {
	      server_ptr = queue0_list.front;
	      status = 1;
	      queue0_list.front = queue0_list.front -> next;
	      server_ptr -> time_in_queue = time - server_ptr -> actual_time;
	      server_ptr -> time_out = time + server_ptr -> time_in_queue + server_ptr -> service;

	    }

	}
      
    }


  return 0;
}

