#include <stdio.h>
#include <stdlib.h>
#include "table_display.h"


void print_per_process(Process_Info* process, pid_t pid) {
    printf("        PID        FD\n");
    printf("      =================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            if(pid == -1){
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
    printf("        PID        FD         Filename\n");
    printf("      ===================================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
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
    printf("        FD         Inode\n");
    printf("      ====================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
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
    printf("        PID        FD         Filename             Inode\n");
    printf("      ====================================================\n");
    
    int row_num = 1;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if(pid != -1 && pid != p->pid){
            continue;
        }
        for (FD_Entry* fd = p->fd_list; fd != NULL; fd = fd->next, row_num++) {
            if(pid == -1){
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
    printf("        Summary Table\n");
    printf("      =================\n");

    for(Process_Info* p = process; p != NULL; p = p->next){
        printf("%d (%d), ", p->pid, p->fd_size);
    }
    printf("\n\n");
    return;
}

void print_threshold(Process_Info* process, int threshold){
    printf("## Offending processes:\n");

    int has_exceeded = 0;
    for (Process_Info* p = process; p != NULL; p = p->next) {
        if (p->fd_size > threshold) {
            printf("%d (%d), ", p->pid, p->fd_size);
            has_exceeded++;
        }
    }

    if(has_exceeded == 0){
        printf("None");
    }

    printf("\n\n");
    return;

}