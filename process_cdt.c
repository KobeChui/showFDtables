#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include <string.h>

Process_Info* create_new_process(pid_t pid){
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
    if(fd_entry == NULL || process == NULL){
        return -1;
    }
    if (process->fd_list == NULL) {
        process->fd_list = fd_entry;
        return -1;
    } 
    
    FD_Entry* temp = process->fd_list;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = fd_entry;
    
    return 0;
}

void free_process_info(Process_Info* root){
    while (root != NULL){
        FD_Entry* fd_root = root->fd_list;
        while (fd_root != NULL){
            FD_Entry* temp = fd_root;
            fd_root = fd_root->next;
            free(temp);
        }
        Process_Info* temp_process = root;
        root = root->next;
        free(temp_process);
    }
    return;
}