#! /bin/bash

reset='\033[0m'             # reset
                            # colors
red='\033[0;31m'            # red
green='\033[0;32m'          # green

# compile
make 2>&1 > /dev/null

function memory_leaks {
    # get the nubmer of arguments
    number_of_arguments=$#
    # check the number of arguments
    if [[ $number_of_arguments -ne "1" ]]
    then
        echo -e "${red}Wrong number of arguments: './memory_leak.sh executable' ${reset}"
        return
    fi

    # get executable
    executable=$1

    output=$(valgrind --leak-check=full --track-origins=yes ./"$executable" 2>&1 > /dev/null)
    total_heap_usage=$(echo "$output" | grep "total heap usage")
    allocs=$(echo "$total_heap_usage" | awk '{ print $5 }')
    frees=$(echo "$total_heap_usage" | awk '{ print $7 }')

    if (( "$allocs" == "$frees" ));
    then
        echo -e "${green}${executable}: All heap blocks were freed -- no leaks are possible${reset}"
    else
        echo -e "${red}${executable}: Allocated $allocs times but freed only $frees${reset}"
    fi
}

# function calls
memory_leaks simple_job
memory_leaks multiple_jobs
memory_leaks global_variable
memory_leaks pause_resume
memory_leaks jobs_add_jobs
memory_leaks dynamic_barrier

# clean
make clean 2>&1 > /dev/null

exit 0