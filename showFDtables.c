#include <stdio.h>
#include <stdlib.h>
#include "parse_command_line.h"
#include "table_display.h"
#include "process.h"


int main(int argc , char** argv){
    //Retrieve all the indicated flags.
    flags flag;
    parse_arguments(&flag, argc, argv);

    printf("pid %d\n", flag.pid); printf("--per-process %d\n", flag.process); printf("--systemWide %d\n", flag.system_wide); printf("--Vnodes %d\n", flag.vnodes); printf("--composite %d\n", flag.composite); printf("--summary %d\n", flag.summary); printf("--threshold= %d\n", flag.threshold);

    //Retrieve all information on every single process.
    Process_Info* process_list = collect_process();
    if(process_list == NULL){
        fprintf(stderr, "No process found.\n");
        exit(1);
    }

    //Display tables according to given flags
    if(flag.process){
        print_per_process(process_list, flag.pid);
    }
    if(flag.system_wide){
        print_system_wide(process_list, flag.pid);
    }
    if(flag.vnodes){
        print_vnode(process_list, flag.pid);
    }
    if(flag.composite){
        print_composite(process_list, flag.pid);
    }
    if(flag.summary){
        print_summary(process_list);
    }
    if(flag.threshold >= 0){ //FD starts from 0
        print_threshold(process_list, flag.threshold);
    }
    //If not a single flag is given, print all tables except summary and threshold tables.
    if(!(flag.process || flag.system_wide || flag.vnodes || flag.composite || flag.summary || flag.threshold != -1)){
        print_per_process(process_list, flag.pid);
        print_system_wide(process_list, flag.pid);
        print_vnode(process_list, flag.pid);
        print_composite(process_list, flag.pid);
    }

    //Free process_list as its memory is dynamically allocated.
    free_process_info(process_list);

    return 0;
}