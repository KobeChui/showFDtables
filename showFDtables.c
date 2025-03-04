#include <stdio.h>

#include "parse_command_line.h"
#include "process.h"

int main(int argc , char** argv){
    flags flag;
    parse_arguments(&flag, argc, argv);

    printf("pid %d\n", flag.pid); printf("--per-process %d\n", flag.process); printf("--systemWide %d\n", flag.system_wide); printf("--Vnodes %d\n", flag.vnodes); printf("--composite %d\n", flag.composite); printf("--summary %d\n", flag.summary); printf("--threshold= %d\n", flag.threshold);

    Process_Info* process_list = collect_process();
    print_table(process_list);
    free_process_info(process_list);

    return 0;
}