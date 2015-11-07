#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#define MICRO_SEC_IN_SEC 1000000

#define SHARED_MEM_BUF 128
#define NUMBER_OF_BUFFERS 100
#define OUR_BUFSIZ SHARED_MEM_BUF

#define FILE_OUTPUT "output.txt"
#define FILE_INPUT "2M.txt"

#define S_KEY 1234  // Exclusivity (remove for Part 2)
#define E_KEY 5678  // Number of Free Buffers
#define N_KEY 9012  // Number of Filled Buffers
#define T_KEY 3456  // Used to ensure Producer accesses File Size before Consumer reads

#define S_LOCKOUT_NUMBER 1
#define E_LOCKOUT_NUMBER (NUMBER_OF_BUFFERS - 1)
#define N_LOCKOUT_NUMBER 0
#define T_LOCKOUT_NUMBER 0

#define SHM_KEY 7890

typedef struct {
    int count;
    char buffer[SHARED_MEM_BUF];
} shared_mem_st;

typedef struct {
    int file_size;
    shared_mem_st shared_mem[NUMBER_OF_BUFFERS];
} circular_buf_st;
