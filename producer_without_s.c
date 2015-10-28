#include "config.h"
#include "semaphore.h"

void main(int argc, char *argv[]) {

    int file_input;
    int shmid;
    int bytes_copied, buf_index = 0;
    char data[OUR_BUFSIZ + 1];
    void *shared_memory = (void *)0;   // Set to null pointer (for now)
    circular_buf_st *shared_buffer;

    /* Semaphores */
    int sem_e_id = semget(E_KEY, 1, 0666 | IPC_CREAT);
    int sem_n_id = semget(N_KEY, 1, 0666 | IPC_CREAT);
    int sem_y_id = semget(Y_KEY, 1, 0666 | IPC_CREAT);

    if ( !set_semvalue(sem_e_id, E_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore E\n");
        exit(EXIT_FAILURE);
    }
    
    if ( !set_semvalue(sem_n_id, N_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore N\n");
        exit(EXIT_FAILURE);
    }   
    
    if ( !set_semvalue(sem_y_id, Y_LOCKOUT_NUMBER) ) {
        fprintf(stderr, "Failed to Initialize Semaphore Y\n");
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
    
    
    
    struct stat st;
    stat(FILE_INPUT, &st);
    shared_buffer -> file_size = st.st_size;
    printf("File Size: %d\n", shared_buffer -> file_size);
    sem_signal(sem_y_id);
    
    while( (bytes_copied = read(file_input, data, OUR_BUFSIZ)) != 0 ){
        data[OUR_BUFSIZ] = '\0';
        sem_wait(sem_e_id);
        
        printf("Bytes: %d, Data: \"%s\"\n", bytes_copied, data);
        printf("Buffer Index: %d\n\n", buf_index);
        
        shared_buffer -> shared_mem[buf_index].count = bytes_copied;    // Set the count of copied bytes
        strcpy( shared_buffer -> shared_mem[buf_index].buffer, data );  // Copy the read data
        if (++buf_index == NUMBER_OF_BUFFERS) buf_index = 0;            // Increment buffer index
        
        sem_signal(sem_n_id);
        
        memset(data, '\0', sizeof(data));
    }
    
    close(file_input);
    exit(EXIT_SUCCESS);
}
