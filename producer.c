#include "config.h"
#include "semaphore.h"

void main(int argc, char *argv[]) {

    int file_input;
    int shmid;
    int bytes_copied, buf_index = 0;
    char data[OUR_BUFSIZ];
    char file_data[BUFSIZ];
    void *shared_memory = (void *)0;   // Set to null pointer (for now)
    circular_buf_st *shared_buffer;
    struct timeval start, end;

    /* Semaphores */
    int sem_s_id = semget(S_KEY, 1, 0666 | IPC_CREAT);
    int sem_e_id = semget(E_KEY, 1, 0666 | IPC_CREAT);
    int sem_n_id = semget(N_KEY, 1, 0666 | IPC_CREAT);
    int sem_t_id = semget(T_KEY, 1, 0666 | IPC_CREAT);

    if ( !set_semvalue(sem_s_id, S_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore S\n");
        exit(EXIT_FAILURE);
    }
    
    if ( !set_semvalue(sem_e_id, E_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore E\n");
        exit(EXIT_FAILURE);
    }
    
    if ( !set_semvalue(sem_n_id, N_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore N\n");
        exit(EXIT_FAILURE);
    }   
    
    if ( !set_semvalue(sem_t_id, T_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore T\n");
        exit(EXIT_FAILURE);
    }
    
    
    /* Shared Memory */
    if ( (shmid = shmget((key_t)SHM_KEY, sizeof(circular_buf_st), 0666 | IPC_CREAT)) == -1 ) {
        fprintf(stderr, "Shared Memory Get Failed\n");
        exit(EXIT_FAILURE);
    }
    
    if ( (shared_memory = shmat(shmid, (void *)0, 0)) == ((void *)-1) ) {
        fprintf(stderr, "Shared Memory Attach Failed\n");
        exit(EXIT_FAILURE);
    }
    shared_buffer = (circular_buf_st *) shared_memory;
    
    
    /* Producer */
    if ( (file_input = open(FILE_INPUT, O_RDONLY)) == -1) {
        fprintf(stderr,"Failed to Open File Input\n");
        exit(EXIT_FAILURE);
    }
    
    /* gets file stats */
    struct stat st;
    stat(FILE_INPUT, &st);
    shared_buffer -> file_size = st.st_size;
    printf("File Size: %d\n", shared_buffer -> file_size);
    sem_signal(sem_t_id);
    
    //Start Time
    gettimeofday(&start, NULL);
    while( (bytes_copied = read(file_input, file_data, BUFSIZ)) != 0 ) {
        if (bytes_copied < 0) {
            fprintf(stderr, "Error when reading from file\n");
            exit(EXIT_FAILURE);
        }	    

	    //size of data already copied to shared mem
	    int size = 0;
	    
	    while(bytes_copied > size){
	        //copying files from read file to a buffer that will be sent to shared memory.
	        strncpy(data, file_data+size, OUR_BUFSIZ);
       
            size += OUR_BUFSIZ;
            int to_copy = OUR_BUFSIZ;

            //checking case where the data isn't OUR_BUFSIZ size
            if(size>bytes_copied)
                to_copy -=(size-bytes_copied);
            sem_wait(sem_e_id);
            sem_wait(sem_s_id);

	        //clearing shared memory
	        shared_buffer -> shared_mem[buf_index].count = 0;
	        memset( shared_buffer -> shared_mem[buf_index].buffer, 0, sizeof(shared_buffer -> shared_mem[buf_index].buffer));	

	        //adding data to shared memory
            shared_buffer -> shared_mem[buf_index].count = to_copy;    // Set the count of copied bytes
            strncpy( shared_buffer -> shared_mem[buf_index].buffer, data, to_copy );  // Copy the read data
        
            sem_signal(sem_s_id);
            sem_signal(sem_n_id);
            if (++buf_index == NUMBER_OF_BUFFERS) buf_index = 0;            // Increment buffer index
            memset(data, '\0', sizeof(data));
        }
    }
    //End Time
    gettimeofday(&end, NULL);   
    printf("Number of Bytes Copied to shared Memory = %d\n", st.st_size);
    printf("Producer Elapsed Time: %ld microseconds\n\n", ((end.tv_sec * MICRO_SEC_IN_SEC + end.tv_usec) 
        - (start.tv_sec * MICRO_SEC_IN_SEC + start.tv_usec)));
    close(file_input);
    exit(EXIT_SUCCESS);
}
