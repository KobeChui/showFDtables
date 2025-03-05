#ifndef DISPLAY_TABLE_H
#define DISPLAY_TABLE_H

#include "process.h"

void print_per_process(Process_Info* process, int has_row_num);

void print_system_wide(Process_Info* process, int has_row_num);

void print_vnode(Process_Info* process, int has_row_num);

void print_composite(Process_Info* process, int has_row_num);

void print_summary(Process_Info* process);

void print_threshold(Process_Info* process, int threshold);

#endif