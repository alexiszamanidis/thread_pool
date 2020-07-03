#include "../../header/header.h"
#include "../../header/queue.h"
#include "../header/queue_test_functions.h"

void add_test(CU_pSuite pSuite, const char* strName, CU_TestFunc pTestFunc) {
    if (NULL == CU_add_test(pSuite, strName, pTestFunc)) {
        CU_cleanup_registry();
        exit(FAILURE);
    }
}

void initialize_queue_test() {
    struct queue *queue = initialize_queue();

    CU_ASSERT( queue->head == NULL );
    CU_ASSERT( queue->tail == NULL );
    CU_ASSERT( queue->length == 0 );

    free_queue(&queue);
}

void push_head_queue_test() {
    struct queue *queue = initialize_queue();

    // queue[(4,4),(3,3),(2,2),(1,1),(0,0)]
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_head_queue(&queue, (void*)test_coordinates);
    }

    struct queue_node *queue_node = queue->tail;

    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = queue_node->data;

        CU_ASSERT( test_coordinates->x == i );
        CU_ASSERT( test_coordinates->y == i );

        queue_node = queue_node->previous;
    }

    free_queue(&queue);
}

void push_tail_queue_test() {
    struct queue *queue = initialize_queue();

    // queue[(0,0),(1,1),(2,2),(3,3),(4,4)]
    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_tail_queue(&queue, (void*)test_coordinates);
    }

    struct queue_node *queue_node = queue->head;

    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = queue_node->data;

        CU_ASSERT( test_coordinates->x == i );
        CU_ASSERT( test_coordinates->y == i );

        queue_node = queue_node->next;
    }

    free_queue(&queue);
}

void pop_head_queue_test() {
    struct queue *queue = initialize_queue();
    
    // queue[(0,0),(1,1),(2,2),(3,3),(4,4)]
    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_tail_queue(&queue, (void*)test_coordinates);
    }

    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = pop_head_queue(&queue);

        CU_ASSERT( test_coordinates->x == i );
        CU_ASSERT( test_coordinates->y == i );

        free_pointer(&test_coordinates);
    }

    free_queue(&queue);
}

void pop_tail_queue_test() {
    struct queue *queue = initialize_queue();

    // queue[(4,4),(3,3),(2,2),(1,1),(0,0)]
    for(int i = 0 ; i < 5 ; i++) {
        struct coordinates *test_coordinates = malloc(sizeof(struct coordinates));
        test_coordinates->x = i;
        test_coordinates->y = i;
        push_head_queue(&queue, (void*)test_coordinates);
    }

    for(int i = 0 ; i < SIZE ; i++) {
        struct coordinates *test_coordinates = pop_tail_queue(&queue);

        CU_ASSERT( test_coordinates->x == i );
        CU_ASSERT( test_coordinates->y == i );

        free_pointer(&test_coordinates);
    }

    free_queue(&queue);
}