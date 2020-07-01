#include "../../header/header.h"
#include "../../header/queue.h"
#include "../../header/job_scheduler.h"
#include "../header/test_functions.h"

struct coordinates {
    int x;
    int y;
};

int main(void) {
    struct queue *queue = initialize_queue();
    
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_queue(&queue, (void*)test_coordinates);
    }

    for(int i = 0 ; i < 5 ; i++) {
        struct queue_node *queue_node = pop_queue(&queue);
        struct coordinates *test_coordinates = queue_node->data;
        printnl(test_coordinates->x);
        printnl(test_coordinates->y);
        free_queue_node(&queue_node);
    }

    free_queue(&queue);

    return SUCCESS;
}