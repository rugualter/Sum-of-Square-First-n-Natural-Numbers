#include <pthread.h>	//used for  pthreads
#include <stdio.h>		//used for printf
#include <stdlib.h>		//used for malloc

int nt,n,nbloco;																		// Defection of base variables requested
int resultado_esperado = 0, *matriz, *buffer,sinal_saida,soma_global, items_buffer=0; 	// Defection of the variables Udadas for calculation and control of the program

pthread_mutex_t indice_mutex, criacao_thread_mutex,buffer_mutex,soma_final_mutex; 		// Mutex Defensement for Exclusivity, a Mutex SO can be used to ensure that there is no unlocking confusions opted for exclusive mutex creatation for operations

// If you use the program with the SCHED YIELD
// Disable Conditions Produce and Consume

pthread_cond_t cond/*, produzir, consumir*/;												// Defemnification of conditions for signaling created threads and full buffer signaling (faster than using the Sched Yield The two options are affordable)

static int numero_para_comeco = 0;  													// Control number to know which block to thread will process

typedef struct {
	
// This structure will pass the values ​​for the threads
// values ​​that will differ from a thread to
// The other, give themselves within the structure.

  int inicio_inicial;
  int id_threads;

}estrutura_geral;

// This first function represents the function of tasks
// Calculators is divided into two parts a first
// processing the associated block of start according to the sequence of order number
// (Each Thread Obriagtorization Computes a block)
// and a second that allows any thread to grasp a new block
// This function] has a pointer that directs to the structure

void *calculadoras(void *es) 										
{
        int i, inicio, fim, contador;											// Definition of variables
        int calculo_parcial = 0;									// Defention variable partial sum
		
        pthread_mutex_lock(&criacao_thread_mutex);					// Mutex lock to ensure reading the structure and shipping of the created thread sign
        
        estrutura_geral * const estrutura = es;						// Pointer is created for the structure
		
        int const id_threads = estrutura->id_threads;				// structure elonet reading
        int const inicio_inicial = estrutura->inicio_inicial;
		
        contador=0;													// Initialization of Conathdore
        
		if ((inicio_inicial+nbloco) < n){							// Check the block size to be calculated
			fim = inicio_inicial + nbloco;
		}else{
			fim = n;
		}
		
		pthread_cond_signal(&cond);									// Signal to Main Thread that thread was created
		
        pthread_mutex_unlock(&criacao_thread_mutex);				// mutex unlock for the condition variable on the main thread is used
		
        sched_yield();												// It is not necessary to be here but when I assure you that other threads have even more opportunity.
		
     // printf ("the thread% d is calculating the% d at% dn" block, id threads, initial start + 1, end);

		// first part of the computing where the values ​​for
		// sum pasted or brats by order of thread creature
                
		// Pthread Mutex Lock (& ​​Buffer Mutex); // (when to use the conditions or do the complete block calculation) we are entering a region where computing will be fair we need to boque the mutex so that there is no loss of data
				 while(items_buffer==nt){							
				 
				 // while the buffer is equal to the number of tasks
				// There are two options or thread sends a sign to the thread that
				// Sum the buffer values ​​to clean the buffer
				// or then we can choose not to send the signal unlock the mutex
				// and simply release the task with Sched Yield
				// the signal ends as effective because it is not sure that with the Sched Yield
				// The next task is the sum
					 
                            //pthread_cond_wait(&produzir,&buffer_mutex);
							//pthread_mutex_unlock(&buffer_mutex);				// activate when the mutex is previously blocked
							sched_yield();										// To activate the Sched Yield disable the condition and activate the mutex
                        }
  
					for (i = inicio_inicial; i < fim ; i++) {
                       
                        
                        calculo_parcial+= matriz[i]*matriz[i];			// Partial sum calculation
						contador=contador+1;							// Conatra element added
						
                }
				
				pthread_mutex_lock (&buffer_mutex);						// Critical Region ensures that it has exclusivity to record the data
				
				buffer[items_buffer+1]=calculo_parcial;					// Sending Items to Buffer
				items_buffer=items_buffer+1;							// Item Contador No Buffer
				
                //printf("o meu buffer tem %d items a soma parcial é de %d e o buffer tem %d\n",items_buffer,calculo_parcial,buffer[items_buffer]);
                //printf ("A thread %d calculou o bloco de %d a %d\n", id_threads, inicio_inicial+1, fim); 
				
				pthread_mutex_unlock(&buffer_mutex);					// Unlock Mutex to release the processor to the other threads
                //pthread_cond_signal(&consumir);							// Sign the thread adder that there are items in the buffer once again we could use the Sched Yield, which would not be so effective
                sched_yield(); 											// can activate the SCHED YIED for this to disable the condition

				// from this time if you exit blocks
				// by computing the vao threads grab a new block and computing
				// follows exactly the same structure indicated above
				// but now there is no obligation of each thread having a block
				
        while (1) {
                
                pthread_mutex_lock(&indice_mutex);
				
                if (numero_para_comeco >= n) {
                        pthread_mutex_unlock(&indice_mutex);
                        break;
                }
                
                inicio = numero_para_comeco;

                if ((inicio + nbloco) < n)
                        numero_para_comeco = fim = inicio + nbloco;
                else 
                        numero_para_comeco = fim = n;

                
                pthread_mutex_unlock(&indice_mutex);
				
                calculo_parcial = 0;						// Initialization of the partial sum back to 0					
				
                //printf ("A thread %d está a calcular o bloco de %d a %d\n", id_threads, inicio+1, fim); 
                
                    //pthread_mutex_lock (&buffer_mutex);
					
					while(items_buffer==nt){
						
                            //pthread_cond_wait(&produzir,&buffer_mutex);
							//pthread_mutex_unlock(&buffer_mutex);
							sched_yield();								// To activate the Sched Yield disable the condition and activate the mutex
                        }
						
					for (i = inicio; i < fim ; i++) {
   
                        calculo_parcial+= matriz[i]*matriz[i];
						contador=contador+1;							// Conatra element added

                }
				
				pthread_mutex_lock (&buffer_mutex);
				
				buffer[items_buffer+1]=calculo_parcial;
				items_buffer=items_buffer+1;
				
                //printf("o meu buffer tem %d items a soma parcial é de %d e o buffer tem %d\n",items_buffer,calculo_parcial,buffer[items_buffer]);
                //printf ("A thread %d calculou o bloco de %d a %d\n", id_threads, inicio+1, fim);
				
                pthread_mutex_unlock (&buffer_mutex);
                //pthread_cond_signal(&consumir);
                sched_yield();						// can activate the SCHED YIED for this to disable the condition
        }
		
        sinal_saida=sinal_saida+1;						// Form of signal that Thread left so that the thread of sum and that cleans the buffer knows that it can end
        
		printf("tarefa %d calculou %d elementos\n",id_threads,contador);
		//printf("tarefa %d de saída\n",id_threads);
		
		pthread_exit(NULL);

}

// Here is presented the function that adds the sums pariases that are in the buffer and the clean

void *somadora(void *ts) 
{
 
	pthread_mutex_lock(&criacao_thread_mutex);			// We block the mutex so that the signal is given that the thread was created
      
    //printf("Sou a thread somadora\n");
    
	pthread_cond_signal(&cond);							// We signal the Main Thread that Thread was created
        
    pthread_mutex_unlock(&criacao_thread_mutex);		// Unlock Mutex so that threads are at will
        
    pthread_mutex_lock(&buffer_mutex);					// We stray in a critical operation where the data can not be lost, we block Mutex
	
		while(items_buffer==0){
			
			// While buffer has 0 elonides
			// We signal the threads that can produce
			// is then a standby condition or
			// We can use a Sched Yield
			
            //pthread_cond_wait(&consumir,&buffer_mutex);
            pthread_mutex_unlock(&buffer_mutex);				// To activate the Sched Yield disable the condition and activate the mutex
            sched_yield();
        }
		
        while(sinal_saida<nt){						// While all thread do not extinguish this condition is valid
           
            while(items_buffer!=0){					// Whenever the buffer is different from 0 is calculated the sum of the partial sums and the buffer is emptied
                
                soma_global+=buffer[items_buffer];	// Update of the global sum
                items_buffer=items_buffer-1;		//reduçao do buffer
				
                //printf("o meu buffer ficou com %d items\n",items_buffer);
                
            }
			
            pthread_mutex_unlock(&buffer_mutex);	// Computing Conducted We Can Unlock Mutex
            //pthread_cond_signal(&produzir);			// Sending signal that threads can produce more partial sums
            sched_yield();						// can activate the SCHED YIED for this to disable the condition
        }
		
		// When all thread finished
		// The sum task will have to rotate one more
		// to check if there are no elements in the mutex buffer
		// Logic is the same before
		
        pthread_mutex_lock(&soma_final_mutex);		
		
        while(items_buffer!=0){
			
                soma_global+=buffer[items_buffer];
                items_buffer=items_buffer-1;
				
                //printf("o meu buffer ficou com %d items\n",items_buffer);
            }
			
        pthread_mutex_unlock(&soma_final_mutex);
		
        //printf("Sou a thread somadora estou de saida\n");
        
        pthread_exit(NULL);

}

// Principle Function

int main(int argc, char *argv[])
{
        int i,z;							// Variability Defineration
		
		// Collection of command line elements
		
		nt=atoi(argv[1]);
		n=atoi(argv[2]);
		nbloco=atoi(argv[3]);
		
		// verification of the elements incerented by the user
			
		if(argc!=4){
			printf("Utilização: ./mtss nt n nbloco\n");
			exit(1);}
	  
		if(nt<1){
			printf("O numero de processos terá que ser pelo menos 1\n");
			exit(1);}
	  
		if(n<1||n>999){
		printf("O n tem que estar comprefimido entre 1 e 999\n");
		exit(1);}
  
		if(nbloco<1){
		printf("O bloco tem que ser pelo menos 1\n");
		exit(1);
		}

		printf("Soma do quadrado dos %d primeiros numeros naturais com %d tarefas e blocos de %d termos\n",n,nt,nbloco);
	
      // Defety for threads and atttributes
		
		pthread_t threads_calculadora[nt];
        pthread_t thread_soma;
        pthread_attr_t attr;
		
		// Allocate space for the structure that will be passed to threads
		
		estrutura_geral * estrutura = malloc(sizeof(estrutura_geral));
		
		// Allocate space for matrix with calculation values ​​and for buffer
		
		matriz = malloc(sizeof(int)*n);
		buffer = malloc(sizeof(int)*nt);
        
		// Matrix filling with N values
		
        for(z=0;z<n;z++){
			
            matriz[z]=z+1;
			
        }

       // Initialization of Mutex
		
        pthread_mutex_init(&indice_mutex, NULL);
        pthread_mutex_init(&criacao_thread_mutex,NULL);
        pthread_mutex_init(&soma_final_mutex,NULL);
        pthread_mutex_init(&buffer_mutex,NULL);
		
		//inicializaçao das condicoes
		
        pthread_cond_init(&cond,NULL);
        //pthread_cond_init(&produzir,NULL);					// During the use of Sched Yield
        //pthread_cond_init(&consumir,NULL);					// During the use of Sched Yield
		
		// Initializacao and defenicao of attributes
		
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // This attribute is already predefined but it is never too much to guarantee
		
        numero_para_comeco=nbloco*nt; 		// Definition of variable that controls the number for the thread to start when it is in the While Loop (1)
        estrutura->inicio_inicial=0;       // Initialization of varianvel
		
		// creation of thread sum that had used the sum funcao and reduced the buffer
		
        pthread_create(&thread_soma, &attr, somadora,estrutura);
        pthread_cond_wait(&cond,&criacao_thread_mutex);				// waits for the sign that thread is created
		
		//criaçao das threads calculadoras
		
        for (i=0; i<nt; i++) {
			
                ++estrutura->id_threads;			// thread order number
				
                pthread_create(&threads_calculadora[i], &attr, calculadoras,estrutura);		// create thread
				
                estrutura->inicio_inicial=i*nbloco;	// Define the start of thread
				
                pthread_cond_wait(&cond,&criacao_thread_mutex);	// Hopes that it is signaled that the thread was created
        }

        // Hopes all the threads to end
		
        for (i=0; i<nt; i++) {
			
                pthread_join(threads_calculadora[i], NULL);
				
        }
		
        pthread_join(thread_soma, NULL);
       
        resultado_esperado = (n*(n+1)*((2*n)+1))/6;
		
		printf("Soma Total= %d\n",soma_global);
        printf("Resultado esperado = %d\n",resultado_esperado);
       
	    // Release memory
	   
        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&indice_mutex);
		pthread_mutex_destroy(&criacao_thread_mutex);
		pthread_mutex_destroy(&soma_final_mutex);
		pthread_mutex_destroy(&buffer_mutex);
		//pthread_cond_destroy(&produzir);						// During the use of Sched Yield
		//pthread_cond_destroy(&consumir);						// During the use of Sched Yield
        
		return 0;
}