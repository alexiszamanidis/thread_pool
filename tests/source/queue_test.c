#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../../header/test_functions.h"

struct coordinates {
    int x;
    int y;
};

int main(void) {
    struct generic_queue *generic_queue = initialize_generic_queue(sizeof(int));
    
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_generic_queue(&generic_queue, (void*)test_coordinates);
    }

    for(int i = 0 ; i < 5 ; i++) {
        struct generic_queue_node *generic_queue_node = pop_generic_queue(&generic_queue);
        struct coordinates *test_coordinates = generic_queue_node->data;
        printnl(test_coordinates->x);
        printnl(test_coordinates->y);
        free_generic_queue_node(&generic_queue_node);
    }

    free_generic_queue(&generic_queue);

    return SUCCESS;
}