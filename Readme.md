Assumptions:
- PID always appear before other flags.


char linkPath[PATH_MAX];
snprintf(path, sizeof(path), "/proc/%d/fd", pid);

#define _GNU_SOURCE for readlink and lstat