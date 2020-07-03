#! /bin/bash

# compile
cd job_scheduler && make 2>&1 > /dev/null

cd .. && source ./memory_leaks.sh
# function calls
memory_leaks job_scheduler/simple_job
memory_leaks job_scheduler/multiple_jobs
memory_leaks job_scheduler/global_variable
memory_leaks job_scheduler/pause_resume
memory_leaks job_scheduler/jobs_add_jobs
memory_leaks job_scheduler/dynamic_barrier
memory_leaks job_scheduler/null_function_pointer

# clean
cd source && make clean 2>&1 > /dev/null

exit 0