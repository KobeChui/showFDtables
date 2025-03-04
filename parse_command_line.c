#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "parse_command_line.h"

void parse_arguments(flags* flag, int argc, char** argv){
    flag->pid = -1;
    flag->process = 0;
    flag->system_wide = 0;
    flag->vnodes     = 0;
    flag->composite  = 0;
    flag->summary    = 0;
    flag->threshold  = -1;

    static struct option long_options[] = {
        {"per-process", no_argument, NULL, 'p'},
        {"systemWide", no_argument, NULL, 'w'},
        {"Vnodes", no_argument, NULL, 'v'},
        {"composite", no_argument, NULL, 'c'},
        {"summary", no_argument, NULL, 's'},
        {"threshold", required_argument, NULL, 't'},
        {0,0,0,0}
    };

    int option;
    int option_index = 0;
    opterr = 0;
    char* endptr = NULL;

    while((option = getopt_long(argc, argv, "", long_options, &option_index)) != -1){
        switch (option)
        {
        case 'p':
            flag->process++;
            break;
        case 'w':
            flag->system_wide++;
            break;
        case 'v':
            flag->vnodes++;
            break;
        case 'c':
            flag->composite++;
            break;
        case 's':
            flag->summary++;
            break;
        case 't':
            //Check argument
            if(optarg == NULL){
                fprintf(stderr, "Missing arugment for --threshold.\n");
                exit(1);
            }

            flag->threshold = (int)strtol(optarg, &endptr, 10);
            
            //Check possibly non-digit character
            //If strtol reads a int then endptr must be \0
            if(*endptr != '\0'){
                fprintf(stderr, "Invalid argument for --threshold.\n");
                exit(1);
            }
            break;
        default:
            fprintf(stderr, "Invalid flag provided: %s\n", argv[optind-1]);
            exit(1);
        }
    }

    //Check positional arguments
    //By design of getopt, the positional argument is now at the end of argv
    //Also, optind is now the index after the index of the last flag.
    //Check if there are more than one positional argument
    if(argc - optind > 1){
        fprintf(stderr, "Too many positional arguments\n");
        exit(1);
    }

    if(argc > optind){
        flag->pid = strtol(argv[optind], &endptr, 10);
        if(*endptr != '\0'){
            fprintf(stderr, "Invalid positional argument\n");
            exit(1);
        }
    }
}