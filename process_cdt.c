#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include <string.h>

Process_Info* create_new_process(pid_t pid){
    ///_|> descry: Creates a new process by dynamically allocating memory.
    ///_|> pid: Indicates the unique PID for the process to be created, type: pid_t
    ///_|> returning: This function returns a pointer to a new process with PID pid, type: Process_Info*

    Process_Info* new_process = (Process_Info*)malloc(sizeof(Process_Info));
    if(new_process == NULL){
        perror("Malloc New Process");
        exit(1);
    }

    new_process->pid = pid;
    new_process->fd_list = NULL;
    new_process->next = NULL;
    return new_process;
}

FD_Entry* create_new_fd(int fd, char* file_name, ino_t inode){
    //_|> descry: Creates a new FD by dynamically allocating memory.
    ///_|> fd: Indicates the unique FD number for the FD to be created, type: int
    ///_|> file_name: Indicates the name of the file that this FD is pointing at, type: char*
    ///_|> inode: Indicates the corresponding inode that this FD is pointing at, type: ino_t
    ///_|> returning: This function returns a pointer to a new FD with FD number fd, file name file_name, and inode inode, type: FD_Entry*

    FD_Entry* new_fd_entry = (FD_Entry*)malloc(sizeof(FD_Entry));
    if(new_fd_entry == NULL){
        perror("Malloc New FD");
        exit(1);
    }

    new_fd_entry->fd = fd;
    strncpy(new_fd_entry->file_name, file_name, PATH_MAX-1);
    new_fd_entry->file_name[strlen(new_fd_entry->file_name)] = '\0'; 
    new_fd_entry->inode = inode;
    new_fd_entry->next = NULL;
    return new_fd_entry;
}

int add_fd_to_process(FD_Entry* fd_entry, Process_Info* process){
    //_|> descry: Adds an FD entry fd_entry into the linked list of FDs in process.
    ///_|> fd_entry: Indicates the FD entry to be added to process, type: FD_Entry*
    ///_|> process: Indicates the process which its list of FD entries will have fd_entry added, type: Process_Info*
    ///_|> returning: This function returns 0 iff fd_entry is successfully added into fd_list in process, returns -1 otherwise, type: int

    if(fd_entry == NULL || process == NULL){
        return -1;
    }
    if (process->fd_list == NULL) { //Empty linked list
        process->fd_list = fd_entry;
        return 0;
    } 
    
    FD_Entry* temp = process->fd_list;

    //Add to tail of linked list
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = fd_entry;
    
    return 0;
}

void free_process_info(Process_Info* root){
    //_|> descry: Free all memory associated with root.
    ///_|> root: Indicates the head of a linked list of processes, type: Process_Info*
    ///_|> returning: This function returns nothing

    while (root != NULL){
        //Free FDs for each process
        FD_Entry* fd_root = root->fd_list;
        while (fd_root != NULL){
            FD_Entry* temp = fd_root;
            fd_root = fd_root->next;
            free(temp);
        }

        //All FDs freed, free process
        Process_Info* temp_process = root;
        root = root->next;
        free(temp_process);
    }
    return;
}