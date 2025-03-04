#ifndef PARSE_H
#define PARSE_H

typedef struct Flags{
    int pid;
    int process;
    int system_wide;
    int vnodes;
    int composite;
    int summary;
    int threshold;
}flags;

void parse_arguments(flags* flag, int argc, char** argv);

#endif