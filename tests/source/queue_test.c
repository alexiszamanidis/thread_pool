#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/job_scheduler_test_functions.h"

struct coordinates {
    int x;
    int y;
};

int main(void) {
    struct queue *queue = initialize_queue();
    
    // push tail: queue[(0,0),(1,1),(2,2),(3,3),(4,4)]
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_tail_queue(&queue, (void*)test_coordinates);
    }

    // pop head
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = pop_head_queue(&queue);
        printnl(test_coordinates->x);
        printnl(test_coordinates->y);
        free_pointer(&test_coordinates);
    }

    printf("\n");

    // push head: queue[(4,4),(3,3),(2,2),(1,1),(0,0)]
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_head_queue(&queue, (void*)test_coordinates);
    }

    // pop tail
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = pop_tail_queue(&queue);
        printnl(test_coordinates->x);
        printnl(test_coordinates->y);
        free_pointer(&test_coordinates);
    }

    free_queue(&queue);

    return SUCCESS;
}