#! /bin/bash

echo "Queue Tests"

# compile queue tests
cd ../queue && make 2>&1 > /dev/null
# run queue tests
cd ../scripts && source ./cunit.sh
cunit ../queue/main

# queue clean
cd ../queue && make clean 2>&1 > /dev/null

#####################################################################

echo "Job scheduler Tests"
# compile job scheduler tests
cd ../job_scheduler && make 2>&1 > /dev/null
# run job scheduler tests
cd ../scripts && source ./memory_leaks.sh
memory_leaks ../job_scheduler/simple_job
memory_leaks ../job_scheduler/multiple_jobs
memory_leaks ../job_scheduler/global_variable
memory_leaks ../job_scheduler/pause_resume
memory_leaks ../job_scheduler/jobs_add_jobs
memory_leaks ../job_scheduler/dynamic_barrier
memory_leaks ../job_scheduler/null_function_pointer

# job scheduler clean
cd ../job_scheduler && make clean 2>&1 > /dev/null

exit 0