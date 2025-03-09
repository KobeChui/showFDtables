#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <linux/limits.h>

typedef struct FD_Entry_Struct{
    int fd;                         //File descriptor number
    char file_name[PATH_MAX];       //File path
    ino_t inode;                    //Inode number
    struct FD_Entry_Struct* next;   //Store FDs in linked list
} FD_Entry;

typedef struct Process_Info_Struct{
    pid_t pid;
    FD_Entry* fd_list;
    int fd_size;                     //size of fd_list
    struct Process_Info_Struct* next;
} Process_Info;

//process_cdt.c
Process_Info* create_new_process(pid_t pid);

FD_Entry* create_new_fd(int fd, char* file_name, ino_t inode);

int add_fd_to_process(FD_Entry* fd_entry, Process_Info* process);

void free_process_info(Process_Info* root);

void print_table(Process_Info* head);

//process_info.c
Process_Info* collect_fd(pid_t pid);

Process_Info* collect_process();

#endif