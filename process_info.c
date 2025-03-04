#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "process.h"

void print_table(Process_Info* head) {
    printf(" PID     FD       Filename                               Inode\n=====================================================================\n");

    for (Process_Info* proc = head; proc; proc = proc->next) {
        for (FD_Entry* fd = proc->fd_list; fd; fd = fd->next) {
            printf("%-8d %-8d %-40s %-8lu\n", proc->pid, fd->fd, fd->file_name, fd->inode);
        }
    }
}

Process_Info* collect_fd(pid_t pid){
    char pid_fd_path[256];
    snprintf(pid_fd_path, sizeof(pid_fd_path), "/proc/%d/fd", pid);

    DIR* fd_directory = opendir(pid_fd_path);
    if(fd_directory == NULL){
        //We skip those fd that we do not have permission to open
        return NULL; 
    }

    Process_Info* new_process = create_new_process(pid);

    struct dirent* fd_entry;

    while((fd_entry = readdir(fd_directory)) != NULL){
        if(strncmp(fd_entry->d_name, ".", 1) == 0 || strncmp(fd_entry->d_name, "..", 2) == 0){
            continue;
        }

        char* endptr = NULL;
        int fd = (int)strtol(fd_entry->d_name, &endptr, 10);

        char fd_link[PATH_MAX];
        char fd_content[PATH_MAX];

        //Copy and concatenate the path to FD into link
        snprintf(fd_link, sizeof(fd_link), "%s/%s", pid_fd_path, fd_entry->d_name);

        //Use stat.h to get inode
        struct stat fd_stat;

        //Skip if reading failed
        if (lstat(fd_link, &fd_stat) == -1){
            continue;
        }

        ino_t inode = fd_stat.st_ino;

        int content_length = readlink(fd_link, fd_content, sizeof(fd_content)-1);
        if(content_length == -1){
            continue; //Skip the fd that we do not have permission to open
        }

        fd_content[content_length] = '\0';

        FD_Entry* new_fd = create_new_fd(fd, fd_content, inode);
        add_fd_to_process(new_fd, new_process);
    }

    closedir(fd_directory);

    return new_process;
}

Process_Info* collect_process(){
    DIR* process_directory = opendir("/proc");
    if(process_directory == NULL){
        perror("opendir");
        exit(1);
    }
    
    Process_Info* process_list = NULL;
    Process_Info* process_list_tail = NULL;

    struct dirent* process_entry;
   
    while((process_entry = readdir(process_directory)) != NULL){
        // Skip "." and ".."
        // "." represents pointer to current directory
        // ".." represents pointer to parent directory (to navigate backwards)
        if(strncmp(process_entry->d_name, ".", 1) == 0 || strncmp(process_entry->d_name, "..", 2) == 0){
            continue;
        }

        char* endptr = NULL;
        pid_t pid = strtol(process_entry->d_name, &endptr, 10);

        //3 Invalid Cases:
        //  (1) Contains character in d_name, then endptr is a char not '\0'
        //  (2) d_name is a file name, then endptr is the name
        //  (3) d_name could be a non-positive number, which cannot be a pid.
        if (*endptr != '\0' || endptr == process_entry->d_name || pid <= 0) {
            continue;
        }

        //Need to check /proc/pid/fd before actually storing info
        Process_Info* new_process = collect_fd(pid); 

        if(new_process == NULL){
            continue;
        }

        //Keep the root, insert at the tail of list
        if(process_list == NULL){
            process_list = new_process;
            process_list_tail = new_process;
        }
        else{
            process_list_tail->next = new_process;
            process_list_tail = new_process; //Need to update new tail
        }
    }

    closedir(process_directory);
    return process_list;
}