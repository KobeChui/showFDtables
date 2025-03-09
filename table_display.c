#include <stdio.h>
#include <stdlib.h>
#include "table_display.h"


void print_per_process(Process_Info* process, pid_t pid) {
    ///_|> descry: Prints process FD table
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> pid: A specified PID by the user, type: pid_t
    ///_|> returning: This function returns nothing

    printf("        PID        FD\n");
    printf("      =================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            //pid == -1 means it is not specified
            //If pid != -1 then we only proceed if pid matches
            continue;
        }

        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            if(pid == -1){
                //print row num if pid is specified
                printf(" %-6d %-10d %d\n", row_num, p->pid, fd->fd);
                continue;
            }
            printf(" %-6s %-10d %d\n", "", p->pid, fd->fd);
        }
    }
    printf("\n");
    return;
}
void print_system_wide(Process_Info* process, pid_t pid){
    ///_|> descry: Prints system-wide FD table
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> pid: A specified PID by the user, type: pid_t
    ///_|> returning: This function returns nothing

    printf("        PID        FD         Filename\n");
    printf("      ===================================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            //pid == -1 means it is not specified
            //If pid != -1 then we only proceed if pid matches
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            //print row num if pid is specified
            if(pid == -1){
                printf(" %-6d %-10d %-10d %-10s\n", row_num, p->pid, fd->fd, fd->file_name);
                continue;
            }
            printf(" %-6s %-10d %-10d %-10s\n", "", p->pid, fd->fd, fd->file_name);
        }
    }
    printf("\n");
    return;
}

void print_vnode(Process_Info* process, pid_t pid){
    ///_|> descry: Prints vnode FD table
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> pid: A specified PID by the user, type: pid_t
    ///_|> returning: This function returns nothing

    printf("        FD         Inode\n");
    printf("      ====================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            //pid == -1 means it is not specified
            //If pid != -1 then we only proceed if pid matches
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            //print row num if pid is specified
            if(pid == -1){
                printf(" %-6d %-10d %-10ld\n", row_num, fd->fd, fd->inode);
                continue;
            }
            printf(" %-6s %-10d %-10ld\n", "", fd->fd, fd->inode);
        }
    }
    printf("\n");
    return;
}

void print_composite(Process_Info* process, pid_t pid){
    ///_|> descry: Prints a composed view of process FD table, system-wide FD table, and Vnodes FD table
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> pid: A specified PID by the user, type: pid_t
    ///_|> returning: This function returns nothing

    printf("        PID        FD         Filename             Inode\n");
    printf("      ====================================================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            //pid == -1 means it is not specified
            //If pid != -1 then we only proceed if pid matches
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            if(pid == -1){
                //print row num if pid is specified
                printf(" %-6d %-10d %-10d %-10s%-10s %ld\n", row_num, p->pid, fd->fd, fd->file_name, "", fd->inode);
                continue;
            }
            printf(" %-6s %-10d %-10d %-10s%-10s %-10ld\n", "", p->pid, fd->fd, fd->file_name, "", fd->inode);
        }
    }
    printf("\n");
    return;
}

void print_summary(Process_Info* process){
    ///_|> descry: Prints a summarized table indicating the number of FDs open per process.
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> returning: This function returns nothing

    printf("        Summary Table\n");
    printf("      =================\n");

    //Prints in the format of: pid (# of fd), pid (# of fd), ...
    for(Process_Info* p = process; p != NULL; p = p->next){
        printf("%d (%d), ", p->pid, p->fd_size);
    }
    printf("\n\n");
    return;
}

void print_threshold(Process_Info* process, int threshold){
    ///_|> descry: Prints a summarized table indicating the number of FDs open per process.
    ///_|>         Only entries of a number of FDs larger than threshold are printed.
    ///_|> process: A list of processes, type: Process_Info*
    ///_|> threshold: Indicates the maximum of number of FDs, type: int
    ///_|> returning: This function returns nothing

    printf("## Offending processes:\n");

    int has_exceeded = 0;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if (p->fd_size > threshold) {
            printf("%d (%d), ", p->pid, p->fd_size);
            has_exceeded++; //There is at least one entry of pid (# of fd)
        }
    }

    //Prints None if there is no entry of pid (# of fd) whatsoever
    if(has_exceeded == 0){
        printf("None");
    }

    printf("\n\n");
    return;

}