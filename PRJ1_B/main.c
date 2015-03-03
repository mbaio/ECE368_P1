/* This file contains the source code for ECE 368 Project #1 part B on priority queues

  Written by: Michael Baio and Igal Flegmann
  This program takes two types of modes:
    Mode 1: <executable> <lamba_0> <lamba_1> <mu> <# of tasks>
    Mode 2: <executable> <file.txt>  (where file.txt contains lines with the format <arrival time> <priority> <service time>
  
  Disclaimer: this program can only simulate up to 100,000 clients per category 200,000 total
  */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define CONST 1


typedef struct linky {
  long int actual_time;
  int sub_tasks;
  long int * array_sub;
  int priority;
  long int time_out;
  struct linky * next;
  struct linky * buffnext;
  
  long int time_in_queue;
}input_list;

typedef struct queue_length {
  long int length;
  struct queue_length * next;
}queue_len;

typedef struct buffy {
  input_list * front;
  input_list * back;
}buffer_list;


  

input_list * List_merge (input_list *, input_list *);
input_list * generate_input1(float,float,float,int);
input_list * generate_input2(FILE *);
input_list * create_node(long int,int,int,long int *);
void destroy_list (input_list *);
char * * explode(const char *, const char *, int *);
queue_len * create_queue_len ( int);


input_list * create_node(long int actual_time, int sub_tasks, int priority, long int * sublist)
{
  int i;
  input_list * node = malloc(sizeof(input_list));
  node -> array_sub = malloc(sizeof(long int) * sub_tasks);
  for (i = 0; i < sub_tasks; i ++)
    {
      node -> array_sub[i] = sublist[i]; 
    }
  node -> actual_time = actual_time;
  node-> sub_tasks = sub_tasks;
  node -> priority = priority;
  node -> time_in_queue = 0;
  node -> time_out = 0;
  node -> next = NULL;
  node -> buffnext = NULL;
  return node;
}

input_list * generate_input2 (FILE * ptr)
{
  char str[200];
  int alength;
  char ** inputs;
  int arrival;
  int priority;
  int sub_tasks;
  char delims = ' ';
  input_list * input0;
  input_list * current;
  int i;
  long int arr[32];
  if (fgets(str, 200, ptr) != NULL)
    {
      inputs  = explode (str, &delims, &alength);
      arrival = atoi (inputs[0]);
      priority = atoi (inputs[1]);
      sub_tasks = atoi (inputs[2]);
      for ( i = 3; i < alength; i ++)
	{
	  arr[i-3] = atoi(inputs[i]);
	}
      input0 = create_node(arrival, sub_tasks, priority, arr);
      current = input0;
      for (i = 0; i < alength; i++)
	{
	  free(inputs[i]);
	  
	}
      free(inputs);
    }
  else
    {
      return NULL;
    }
  while (fgets(str, 200, ptr) != NULL)
    {
      inputs  = explode (str, &delims, &alength);
      arrival = atoi (inputs[0]);
      priority = atoi (inputs[1]);
      sub_tasks = atoi (inputs[2]);
      for ( i = 3; i < alength; i ++)
	{
	  arr[i-3] = atoi(inputs[i]);
	}
      current -> next = create_node(arrival, sub_tasks, priority, arr);
      current = current -> next;
      for (i = 0; i < alength; i++)
	{
	  free(inputs[i]);
	  
	}     
      free(inputs);
    }
  fclose(ptr);
  return input0;
}

input_list * generate_input1(float lamba_0,float lamba_1,float mu,int total_tasks)
{
   int ind = 0;
   float R;
   long int current_time = 0;
   int intermediate_time = 0;
   int priority = 0;
   int i;
   input_list * input_merged;
   long int arr [32];
   int sub_task;
   srand(time(NULL));
   double Y1;
   double X;
  // R = ((float)rand())/((float)(RAND_MAX));
  /* X = 1;
   while(X == 1){
      X = (double) rand() / (double) RAND_MAX;
    }
    Y1 = (log(1-X)) / (-lamba_0);*/
     
   intermediate_time =  (int)(ceil(-1 / lamba_0 * log((float)rand()/(float)(RAND_MAX)))); //(int)(ceil(-1 / lamba_0 * log(1-((double)rand() / (double)RAND_MAX))));
   sub_task = rand() % 32 + 1;
   for(i = 0; i < sub_task; i++)
     {
       //R = (float)rand()/(RAND_MAX);
     
     
    
       arr[i]  = rand() % 6 + 1;//ceil(Y1); //rand() % 6 + 1;//(int)ceil(-1 / mu * log(R));//(int)(ceil(-1 / mu * log((float)rand()/(float)(RAND_MAX + 1))));//(int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
     }
   input_list * input0 = create_node(intermediate_time, sub_task, priority, arr);
   input_list * current = input0;
   current_time += intermediate_time;
   ind++;
   // srand(time(NULL));  
   //(int)(ceil(-1 / lamba_0 * log((float)rand()/(float)(RAND_MAX + 1))));
   while (ind < total_tasks)
   {
     
     sub_task = rand() % 32 + 1;
    // R = (float)rand()/(float)(RAND_MAX);
     
      
	intermediate_time = (int)(ceil(-1 / lamba_0 * log((float)rand()/(float)(RAND_MAX))));//(int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX))));
     //printf("intermediate for 0: %d\n",intermediate_time);
     for(i = 0; i < sub_task; i++)
     {
       //R = (float)rand()/(float)(RAND_MAX);
      
     
       arr[i]  = rand() % 6 + 1;//ceil(Y1);//(int)ceil(-1 / mu * log(R));//(int)(ceil(-1 / mu * log((float)rand()/(float)(RAND_MAX + 1))));//(int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
     }
     current_time += intermediate_time;
     current -> next = create_node(current_time, sub_task ,priority, arr);
     current = current -> next;
    ind++; 
   }
   ind = 0;
   current_time = 0;
   priority = 1;
   sub_task = rand() % 32 + 1;
   //R = (float)rand()/(float)(RAND_MAX);
   
   
   intermediate_time = (int)(ceil(-1 / lamba_1 * log((float)rand()/(float)(RAND_MAX))));//(int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX)))); 
   
   for(i = 0; i < sub_task; i++)
     {
       
     
  
       arr[i]  = rand() % 6 + 1;//ceil(Y1);//(int)ceil(-1 / mu * log(R));//(int)(ceil(-1 / mu * log((float)rand()/(float)(RAND_MAX + 1))));//(int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
     }
   input_list * input1 = create_node(current_time, sub_task ,priority, arr);
   current = input1;
   current_time += intermediate_time;
   ind++;
      
   while (ind < total_tasks)
   {
     sub_task = rand() % 32 + 1;
     //R = (float)rand()/(float)(RAND_MAX);
     

       
     intermediate_time = (int)(ceil(-1 / lamba_1 * log((float)rand()/(float)(RAND_MAX))));//(int)(ceil(-1 / lamba_1 * log(1-((double)rand() / (double)RAND_MAX)))); 
     //printf("intermediate for 1: %d\n",intermediate_time);
     for(i = 0; i < sub_task; i++)
     {
       //R = (float)rand()/(float)(RAND_MAX);
       
       
       arr[i]  = rand() % 6 + 1;//ceil(Y1);//(int)ceil(-1 / mu * log(R));//(int)ceil(-1 / mu * log(1-((double)rand() / (double)RAND_MAX))); 
     }
     current_time += intermediate_time;
     current -> next = create_node(current_time, sub_task ,priority, arr);
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

queue_len * create_queue_len ( int length)
{
  queue_len * node = malloc (sizeof(queue_len));
  node -> next = NULL;
  node -> length = length;
  return node;
}

int main(int argc, char ** argv)
{
  //Generate mode
  int total_tasks;
  if (argc != 2 && argc != 5)
  {
    printf("\nNot valid option\n");
    return EXIT_FAILURE;
  }
  
  input_list * input = NULL;

  
  if (argc == 2)
  {
    printf("Mode: 2\tSource File: %s\n\n",argv[1]);
    FILE * fptr = fopen(argv[1],"r");
    if (fptr == NULL)
      return EXIT_FAILURE;
    input = generate_input2(fptr);
  }
  else
  {
    printf("Mode: 1\tParameters: %s, %s, %s, %s\n\n",argv[1],argv[2],argv[3],argv[4]);
    float lamba_0 = atof(argv[1]);
    float lamba_1 = atof(argv[2]);
    float mu = atof(argv[3]);
    total_tasks = atoi(argv[4]);
    printf("%f %f %f %d\n",lamba_0, lamba_1, mu, total_tasks);
    input = generate_input1(lamba_0,lamba_1,mu,total_tasks);
  }
  
//    int ind; 
//   int ind2; 
//   int tmax = 0;
//    for (ind = 0; ind < total_tasks * 2; ind++) 
//      { 
//        printf("Task #%d: # Sub tasks = %d Arrival time = %d Priority = %d\n",ind + 1, input -> sub_tasks, input -> actual_time, input -> priority); 
//        for (ind2 = 0; ind2 < input -> sub_tasks; ind2++) 
//    	{ 
// 	  if (tmax < input -> array_sub[ind2]){
// 	    tmax = input -> array_sub[ind2];}
//    	  printf("\tSub Task #%d: %d\n",ind2,input -> array_sub[ind2]); 
//    	} 
//        printf("\n"); 
//        input = input -> next; 
//      } 
//      printf("max subtask length = %d\n",tmax);
  // local declarations
  buffer_list queue0_list;
 input_list * q_temp;
  buffer_list queue1_list;
  input_list * time_ptr = input;
  long int running_time = 0;
  int status = 0;
  long queue0 = 0;
  long int queue1 = 0;
  long int cpu_usage = 0;
  long int avg_waiting0 = 0;
  long int avg_waiting1 = 0;
  int out = 0;
  long int num_0 = 0;
  int i;
  int j;
  long int scounter = 0;
  long int smax = 0; 
  float sum_load = 0;
  long int smin = 1000;
  long int num_1 = 0;
  long int serv_check;
  int serv_arr[64];
  float sum_length = 0; //sum of the lengths of queues used for avg que length
  queue_len * queue_head; //head of linked list containing the avg queue length
  queue_len * queue_current; //keeps track of the back of the queue link list
  input_list * server_ptr = NULL;
  
  queue_head = create_queue_len (0);
  queue_current = queue_head;
    for (i = 0; i < 64; i++)
    {
     serv_arr[i]=0; 
    }
  while (out == 0)
    {
      //printf("running time = %ld",running_time);
      if (status > 0) //status is how many are in use
	{//check server full
	for(serv_check = 0; serv_check < 64; serv_check++)
	{
	 // printf("\n\tserv out = %d",serv_arr[serv_check]);
	    if(serv_arr[serv_check] != 0 && serv_arr[serv_check] <= running_time){
	      serv_arr[serv_check] = 0;
	      status--;
	    }
	}
	//printf("\n\n");
	}
      while (time_ptr != NULL && time_ptr -> actual_time <= running_time)
	{// adding people to the q


	  if (time_ptr -> priority == 0)
	    {
	      if(queue0 == 0){
		queue0_list.front = time_ptr;
	      }
	      else
		{
		  queue0_list.back -> buffnext = time_ptr;
		}
	      queue0_list.back = time_ptr;
	      queue0++;
	      num_0++;
	    }
	  else
	    {
	      if(queue1 == 0){
		queue1_list.front = time_ptr;
	      }
	      else
		{
		  queue1_list.back -> buffnext = time_ptr;
		}
	      queue1_list.back = time_ptr;
	      queue1++;
	      num_1++;
	    }
	  time_ptr = time_ptr -> next;
	queue_current -> next = create_queue_len (queue1 + queue0);
	queue_current = queue_current -> next;
	}//end q input
	
	
      if (status < 64)
	{//check server is empty
	  if (queue0 > 0)
	    {//getting 0 priority into server
	      q_temp = queue0_list.front;
	    while(q_temp != NULL && status < 64)
	    {
	      if(q_temp -> sub_tasks <= 64 - status)
	      {
		server_ptr = q_temp;
		status += server_ptr -> sub_tasks;
		queue0--;
		if (q_temp ==  queue0_list.front)
		{
		  queue0_list.front = queue0_list.front -> buffnext;
		  q_temp = queue0_list.front;
		}
		else 
		{
		 q_temp = q_temp -> buffnext; 
		}
		server_ptr -> time_in_queue = running_time - server_ptr -> actual_time;
		for(i = 0; i < server_ptr -> sub_tasks; i++)
		{
		server_ptr -> array_sub[i]= running_time  + server_ptr -> array_sub[i];
		}
		i = 0;
		smin = 1000;
		smax = 0;
		scounter = 0;
		j = 0;
		while(j < 64 && i != server_ptr -> sub_tasks)
		{
		  if(serv_arr[j] == 0)
		  {
		   serv_arr[j] = server_ptr -> array_sub[i];
		   cpu_usage += server_ptr -> array_sub[i]- running_time;
		   if(smin > server_ptr -> array_sub[i] - running_time)
		     smin =  server_ptr -> array_sub[i] - running_time;
		   if(smax < server_ptr -> array_sub[i] - running_time)
		     smax = server_ptr -> array_sub[i] - running_time;
		   scounter += server_ptr -> array_sub[i] - running_time;
		   //printf("scounter ++ = %d\n",scounter);
		   i++;
		   
		  }
		  j++;
		}
		if (i != 0)
		{
		sum_load += (smax - smin) / (scounter / i);
		avg_waiting0 += server_ptr -> time_in_queue;
		}
	      }
	      else
	      {
		q_temp = q_temp -> buffnext;
	      }
	     }
	    }
	  if (queue1 > 0 && status < 64)
	    {//getting 1 into servers
	    q_temp = queue1_list.front;
	    while(q_temp != NULL && status < 64)
	    {
	      if(q_temp -> sub_tasks <= 64 - status)
	      {
		server_ptr = q_temp;
		status += server_ptr -> sub_tasks;
		queue1--;
		if (q_temp ==  queue1_list.front)
		{
		  queue1_list.front = queue1_list.front -> buffnext;
		  q_temp = queue1_list.front;
		}
		else
		{
		 q_temp = q_temp -> buffnext; 
		}
		server_ptr -> time_in_queue = running_time - server_ptr -> actual_time;
		for(i = 0; i < server_ptr -> sub_tasks; i++)
		{
		server_ptr -> array_sub[i]= running_time  + server_ptr -> array_sub[i];
		}
		i = 0;
		smin = 1000;
		smax = 0;
		scounter = 0;
		j = 0;
		while(j < 64 && i != server_ptr -> sub_tasks)
		{
		  if(serv_arr[j] == 0)
		  {
		   serv_arr[j] = server_ptr -> array_sub[i];
		   cpu_usage += server_ptr -> array_sub[i]- running_time;
		   if(smin > server_ptr -> array_sub[i] - running_time)
		     smin =  server_ptr -> array_sub[i] - running_time;
		   if(smax < server_ptr -> array_sub[i] - running_time)
		     smax = server_ptr -> array_sub[i] - running_time;
		   scounter += server_ptr -> array_sub[i]- running_time;
		   //printf("scounter ++ = %d j = %d i = %d\n",server_ptr -> array_sub[i],j,i);
		  
		   i++;
		  }
		j++;
		}
		sum_load += (smax - smin) / (scounter / i);
		avg_waiting1 += server_ptr -> time_in_queue;
	      }
	      else
	      {
		q_temp = q_temp -> buffnext;
	      }
	      }
	    }
	}
      
      
      if (queue0 == 0 && queue1 == 0 && status == 0 && time_ptr == NULL)
	{
	  out = 1;
	  running_time--;
	}
     // printf("Running time: %d 0: %d 1: %d\n",running_time,queue0,queue1);
      running_time++;
    }

  
  while (queue_head != NULL)
  {
    sum_length += queue_head -> length;
    queue_current = queue_head;
    queue_head = queue_head -> next;
    free(queue_current);
  }
  
  
  while (input != NULL)
  {
    time_ptr = input;
    free(time_ptr -> array_sub);
    input = input -> next;
    free(time_ptr);
  }
   
   
   printf("Average waiting time for 0: %f seconds\n",(float) avg_waiting0 / num_0);
   printf("Average waiting time for 1: %f seconds\n",(float) avg_waiting1 / num_1);
   printf("Average Queue length: %f clients\n", sum_length / (num_0 + num_1));
   printf("Average Utilization of CPU: %f \n",(float) cpu_usage / (64 * running_time));
   printf("Utilization of CPU Percentage: %f %% \n",(float) 100 * cpu_usage / (64 * running_time));
   printf("Load Balancing Factor: %f\n",(float)(sum_load) / (num_0 + num_1));
   /*
  printf("sum waiting 0: %d num_0: %d\n",avg_waiting0 , num_0);
  printf("sum waiting 1: %d num_1: %d\n",avg_waiting1 , num_1);
  printf("sum length: %f running_time: %d\n",sum_length , running_time);
  printf("cpu_usage: %d running_time: %d\n",cpu_usage, running_time);
   */
  destroy_list (input);
  return 0;
}
