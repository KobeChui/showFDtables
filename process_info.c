#define _GNU_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "process.h"

Process_Info* collect_fd(pid_t pid){
    ///_|> descry: Retrieve all necessary FD information from given PID pid.
    ///_|> pid: Indicates the unique PID for the process of which its FD information is to be collected, type: pid_t
    ///_|> returning: This function returns a pointer to a process with PID pid and its corresponding FD information, type: Process_Info*

    //Get path name to fd with the specified PID, and open this directory
    char pid_fd_path[256];
    snprintf(pid_fd_path, sizeof(pid_fd_path), "/proc/%d/fd", pid); 

    DIR* fd_directory = opendir(pid_fd_path);
    if(fd_directory == NULL){
        //We skip those fd that we do not have permission to open
        return NULL; 
    }

    Process_Info* new_process = create_new_process(pid);
    new_process->fd_size = 0;

    //Begins to go through all files in fd_directory
    struct dirent* fd_entry;

    while((fd_entry = readdir(fd_directory)) != NULL){
        // Skip "." and ".."
        // "." represents pointer to current directory
        // ".." represents pointer to parent directory (to navigate backwards)
        if(strncmp(fd_entry->d_name, ".", 1) == 0 || strncmp(fd_entry->d_name, "..", 2) == 0){
            continue;
        }

        char* endptr = NULL;
        int fd = (int)strtol(fd_entry->d_name, &endptr, 10);
        
        //Check if fd is a negative number.
        if (*endptr != '\0' || endptr == fd_entry->d_name || fd < 0) {
            continue;
        }

        //Use PATH_MAX as file directories could be very long in length
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
        
        //We have obtained all necessary information for a FD.
        FD_Entry* new_fd = create_new_fd(fd, fd_content, inode);

        //Update the size of fd_list in new_process if adding new_FD is successful.
        if(add_fd_to_process(new_fd, new_process) == 0){
            new_process->fd_size++;
        }
    }

    closedir(fd_directory);

    return new_process;
}

Process_Info* collect_process(){
    ///_|> descry: Retrieve all process information available and accessible from /proc.
    ///_|> returning: This function returns a pointer to a linked list of processes, type: Process_Info*

    DIR* process_directory = opendir("/proc");
    //Terminating the program because if we can't access /proc then we can't do anything
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