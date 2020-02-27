#! /bin/bash

# compile
cd source && make 2>&1 > /dev/null

cd .. && source ./memory_leaks.sh
# function calls
memory_leaks source/simple_job
memory_leaks source/multiple_jobs
memory_leaks source/global_variable
memory_leaks source/pause_resume
memory_leaks source/jobs_add_jobs
memory_leaks source/dynamic_barrier

# clean
cd source && make clean 2>&1 > /dev/null

exit 0