/*
 * File:lfgraphmain.h
 *  
 *
 * Author(s):
 *   Dr. Sathya Peri <sathya_p@iith.ac.in>
 *   Muktikanta Sa   <cs15resch11012@iith.ac.in>
 *   Nandini Singhal <cs15mtech01004@iith.ac.in>
 *   
 * Description:
 *   Sequential implementation of a graph
 * Copyright (c) 2017.
 * last Updated: 17/10/2017
 *
*/
#include"acycle_lf.cpp"

atomic<long> vertexID;
double seconds;
struct timeval tv1, tv2;
TIME_DIFF * difference;
int NTHREADS, ops;
//pthread_mutex_t lock;


typedef struct infothread{
  long tid;
  Graph G;
}tinfo;


void* pthread_call(void* t)
{
        tinfo *ti=(tinfo*)t;
        long Tid = ti->tid;
        Graph G1=ti->G;
	int u, v;
	int other, res;
 
 	long long int numOfOperations = 10000000000;
 	long long int numOfOperations_addEdge = numOfOperations * 0.25; 		// 25% for add edge
  	long long int numOfOperations_addVertex = numOfOperations * 0.25; 	// 25% for add vertex
  	long long int numOfOperations_removeVertex = numOfOperations *0.1 ; 	// 10% for remove vertex
  	long long int numOfOperations_removeEdge = numOfOperations * 0.1; 	// 10% for remove edge
  	long long int numOfOperations_containsVertex = numOfOperations *0.15; 	// 15% for contains vertex
  	long long int numOfOperations_containsEdge = numOfOperations * 0.15; 	// 15% for contains edge

	long long int total = numOfOperations_addEdge + numOfOperations_addVertex + numOfOperations_removeVertex + numOfOperations_removeEdge + numOfOperations_containsVertex + numOfOperations_containsEdge; 
	
	while(total > 0)
	{
		gettimeofday(&tv2,NULL);
		difference = my_difftime (&tv1, &tv2);

		if(difference->secs >= seconds)
			break;

		int other=rand()%6;	
	        if(other == 0) 
		{
			if(numOfOperations_addEdge > 0)
   			{	      
		l1:		u = (rand() % (vertexID));		//vertex IDs are from 1
				v = (rand() % (vertexID));
				if(u == v || u == 0 || v == 0)			//simple graph without self loops
					goto l1;
			       // pthread_mutex_lock(&lock);
				res = G1.AddE(u,v); 
				//pthread_mutex_unlock(&lock);
		         	numOfOperations_addEdge = numOfOperations_addEdge - 1;				        
				total = total - 1;
 				ops++;				
      			}
      		}
      		else if(other == 1)
       		{
     			if(numOfOperations_addVertex > 0)
        		{			
				v = vertexID;		//vertices do not come again
				vertexID++;
				//pthread_mutex_lock(&lock);
				res = G1.AddV(v);
				//pthread_mutex_unlock(&lock);
			        numOfOperations_addVertex = numOfOperations_addVertex - 1;
				ops++;				
			        total = total - 1;
        		} 
       		} 
	     	else if(other == 2)
     		{
       			if(numOfOperations_removeVertex > 0)
       			{		        
			l2:	v = rand() % (vertexID);		//dont decrement the total vertex count
				if(v == 0)
					goto l2;
                                //pthread_mutex_lock(&lock);
				res = G1.RemoveV(v);
				//pthread_mutex_unlock(&lock);
			        numOfOperations_removeVertex = numOfOperations_removeVertex - 1;
				ops++;				
			        total = total - 1;
        		} 
       		}
		else if(other == 3)
		{
			if(numOfOperations_removeEdge > 0)
			{
		l3:		u = (rand() % (vertexID));		//vertex IDs are from 1
				v = (rand() % (vertexID));
				if(u == v || u == 0 || v == 0)	
					goto l3;
			        //pthread_mutex_lock(&lock);
				res = G1.RemoveE(u,v); 
				//pthread_mutex_unlock(&lock);
		         	numOfOperations_removeEdge = numOfOperations_removeEdge - 1;				        
				ops++;				
				total = total - 1;
			}
		}
		else if(other == 4)
		{
			if(numOfOperations_containsVertex > 0)
			{
		l4:		u = (rand() % (vertexID));		//vertex IDs are from 1
				if(u == 0)	
					goto l4;
			        //pthread_mutex_lock(&lock);
				res = G1.ContainsV(u); 
				//pthread_mutex_unlock(&lock);
		         	numOfOperations_containsVertex = numOfOperations_containsVertex - 1;				        
				ops++;				
				total = total - 1;
			}
		}
		else if(other == 5)
		{
			if(numOfOperations_containsEdge > 0)
			{
		l5:		u = (rand() % (vertexID));		//vertex IDs are from 1
				v = (rand() % (vertexID));
				if(u == v || u == 0 || v == 0)		
					goto l5;
                               // pthread_mutex_lock(&lock);
				res = G1.ContainsE(u,v); 
				//pthread_mutex_unlock(&lock);
		         	numOfOperations_containsEdge = numOfOperations_containsEdge - 1;				        
				ops++;				
				total = total - 1;
			}
		}
	} 		//end of while loop
}

int main(int argc, char*argv[])	//command line arguments - #threads, #vertices initially, #time in seconds
{
	Graph sg;


	vertexID.store(1);
	int i;

	if(argc < 3)
	{
		cout << "Enter 3 command line arguments - #threads, #vertices initially, #time in seconds" << endl;
		return 0;
	}

	NTHREADS = atoi(argv[1]);
	int initial_vertices = atoi(argv[2]); 		// initial number of vertices
	seconds = atoi(argv[3]);		
   	ops = 0;

	
	//initGraph();
	vertexID.store(initial_vertices + 1);		// or +1?
	

	//create initial vertices
	//pthread_mutex_init(&lock, NULL);
	
	
	//graph = NULL;

	//initialGraph(initial_vertices);

	sg.create_initial_vertices(initial_vertices);
//	print_graph();

	cout << "Number of Threads: " << NTHREADS << endl;
	cout << "Initial graph with " << initial_vertices << " created." << endl;
        pthread_t *thr = new pthread_t[NTHREADS];
	// Make threads Joinable for sure.
    	pthread_attr_t attr;
   	pthread_attr_init (&attr);
   	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
   	int dig,temp; 
	double duration = 0.0;

	gettimeofday(&tv1,NULL);
	cout << "timer started . . ." << endl;

	for (i=0;i < NTHREADS;i++)
       	{
       	      tinfo *t =(tinfo*) malloc(sizeof(tinfo));
		t->tid = i;
		t->G = sg;
       		pthread_create(&thr[i], &attr, pthread_call, (void*)t);
        }

	for (i = 0; i < NTHREADS; i++)
      	{
		pthread_join(thr[i], NULL);
	}
	

	cout << seconds <<  " seconds elapsed" << endl;

    	cout << "Total operations: " << ops <<endl;

/*
	NodeList *temp1 = graph;
	while(temp1->next != NULL)
	{
		NodeList *temp2 = temp1->next;
		assert(temp1->listhead.key < temp2->listhead.key);

		if(temp1->listhead.next == NULL)
		{
			temp1 = temp1->next;
			continue;
		}
		Node *temp3 = temp1->listhead.next;
		while(temp3->next != NULL)
		{
			Node *temp4 = temp3->next;
			assert(temp3->key < temp4->key);
			temp3 = temp3->next;
		}
		temp1 = temp1->next;
	}

*/
	return 0;
}

