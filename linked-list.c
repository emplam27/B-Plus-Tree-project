#include <stdio.h>
#include <stdlib.h>


typedef struct _node
{
    struct _node *next;
    struct _node *before;
    int data;
} node;

int insert_node(int index, int data, node *head, int *list_length);
int search_node(int value, node *head);

int main()
{
    int list_length = 0;
    node *head = malloc(sizeof(node));
    (*head).next = NULL;

    insert_node(0, 10, head, &list_length);
    insert_node(1, 20, head, &list_length);
    insert_node(2, 40, head, &list_length);
    insert_node(2, 60, head, &list_length);
    insert_node(5, 60, head, &list_length);
    search_node(50, head);

    // 노드들의 값을 출력하는 과정
    node *curr_node = (*head).next;
    while (curr_node)
    {
        printf("%d\n", curr_node->data);
        curr_node = (*curr_node).next;
    }

    // 노드들을 free해주는 과정
    curr_node = (*head).next;
    while (curr_node)
    {
        node *tmp_next = (*curr_node).next;
        free(curr_node);
        curr_node = tmp_next;
    }

    free(head);

    return 0;
}

int insert_node(int index, int data, node *head, int *list_length)
{   
    if (index > *list_length) {
        printf("fail insertion %d in %d index\n", data, index);
        return 0;
    }

    // 삽입되야 할 노드까지 이동
    node *curr_node = head;
    for (int i = 0; i < index; i ++) {
        curr_node = (*curr_node).next;
    }
    
    // 노드 삽입
    node *next_node = (*curr_node).next;
    node *new_node = malloc(sizeof(node));
    (*new_node).data = data;
    (*new_node).next = next_node;
    (*curr_node).next = new_node;
    *list_length += 1;
    printf("success insertion %d in %d index\n", data, index);
}

int search_node(int value, node *head)
{
    node *curr_node = (*head).next;
    int index = 0;
    while ((*curr_node).next)
    {
        if ((*curr_node).data == value) {
            printf("%d is located %d index\n", value, index);
            return 0;
        }
        curr_node = (*curr_node).next;
        index += 1;
    }
    printf("%d is not in linked list\n", value);
}