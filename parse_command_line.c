#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "parse_command_line.h"

int positional_argument_checker(int argc, char** argv){
    ///_|> descry: Checks whether there is a positional argument appearing after one or more flags are indicated.
    ///_|> argc: The number of arguments from the command line, type: int
    ///_|> argv: The list of arguments from the command line, type: char**
    ///_|> returning: This function returns 0 iff no positional arguments appear after flags are indicated, returns -1 otherwise, type: int

    int flag_found = 0;
    for(int i=1; i<argc; i++){
        if(strncmp(argv[i], "--", 2) == 0){ //flag found
            flag_found++;
            continue;
        }
        //Positional argument found
        if(flag_found){ //Checks if there is a flag indicated before this positonal argument
            return -1;
        }
    }
    return 0;
}

void parse_arguments(flags* flag, int argc, char** argv){
    ///_|> descry: A parsing function that reads and stores arguments into flag from the command line.
    ///_|> flag: A struct containing acceptable flags for this program, type: flags*
    ///_|> argc: The number of arguments from the command line, type: int
    ///_|> argv: The list of arguments from the command line, type: char**
    ///_|> returning: This function returns nothing

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
        {"threshold", optional_argument, NULL, 't'},
        {0,0,0,0}
    };

    //Check positional argument placement first
    if(positional_argument_checker(argc, argv) == -1){
        fprintf(stderr, "Positional argument found after flags.\n");
        exit(1);
    }

    int option;
    int option_index = 0;
    opterr = 0;
    char* endptr = NULL;
    
    //Read and identify the flag from argv.
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
            //Check if argument is provided
            if(optarg == NULL || optarg[0] == '\0'){
                fprintf(stderr, "Missing arugment for --threshold.\n");
                exit(1);
            }

            flag->threshold = (int)strtol(optarg, &endptr, 10);
           
            //Check possibly non-digit character, and only accept ints
            //If strtol reads a int then endptr must be \0
            if(*endptr != '\0' || flag->threshold < 0){
                fprintf(stderr, "Invalid argument for --threshold.\n");
                exit(1);
            }
            break;
        default:
            fprintf(stderr, "Invalid flag provided: %s\n", argv[optind-1]);
            exit(1);
        }
    }

    //Check positional argument (pid)
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