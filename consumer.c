#include "config.h"
#include "semaphore.h"

void main(int argc, char *argv[]) {
    
    int file_output;
    int file_size;
    int shmid;
    int running = 1;                    // Change --> filesize check
    int bytes_copied, buf_index = 0;
    char data[OUR_BUFSIZ + 1];
    void *shared_memory = (void *)0;    // Set to null pointer (for now)
    circular_buf_st *shared_buffer;
    
    /* Semaphores */
    int sem_s_id = semget(S_KEY, 1, 0666 | IPC_CREAT);
    int sem_e_id = semget(E_KEY, 1, 0666 | IPC_CREAT);
    int sem_n_id = semget(N_KEY, 1, 0666 | IPC_CREAT);
    int sem_y_id = semget(Y_KEY, 1, 0666 | IPC_CREAT);
    
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

    /* Consumer */
    if ( (file_output = open(
                              FILE_OUTPUT,
                              O_WRONLY | O_CREAT | O_APPEND, 
                              0666)
                            ) == -1){
                            
        fprintf(stderr,"Failed to Open File Output\n");
        exit(EXIT_FAILURE);
    }
    
    sem_wait(sem_y_id);
    file_size = shared_buffer -> file_size;
    
    while(file_size > 0){
        sem_wait(sem_n_id);
        sem_wait(sem_s_id);
        
        strcpy(data, shared_buffer -> shared_mem[buf_index].buffer);
        bytes_copied = shared_buffer -> shared_mem[buf_index].count;
        
        sem_signal(sem_s_id);
        sem_signal(sem_e_id);
        
        if (++buf_index == NUMBER_OF_BUFFERS) buf_index = 0;    // Increment buffer index
        printf("Buffer Index: %d\n", buf_index - 1);
        
        write(file_output, data, bytes_copied);
        file_size -= bytes_copied;
    }
    
    
    close(file_output);
    exit(EXIT_SUCCESS);
}
