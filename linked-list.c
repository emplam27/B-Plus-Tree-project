#include <stdio.h>
#include <stdlib.h>

/*
연결리스트를 구현
*/

typedef struct _NODE
{
    struct _NODE *next;
    struct _NODE *before;
    int data;
} NODE;

int insertNode(int index, int data, NODE *head, int *list_length);
int searchNode(int value, NODE *head);

int main()
{
    int list_length = 0;
    NODE *head = malloc(sizeof(NODE));
    (*head).next = NULL;

    insertNode(0, 10, head, &list_length);
    insertNode(1, 20, head, &list_length);
    insertNode(2, 40, head, &list_length);
    insertNode(2, 60, head, &list_length);
    insertNode(5, 60, head, &list_length);
    searchNode(50, head);

    // 노드들의 값을 출력하는 과정
    NODE *curr_node = (*head).next;
    while (curr_node)
    {
        printf("%d\n", curr_node->data);
        curr_node = (*curr_node).next;
    }

    // 노드들을 free해주는 과정
    curr_node = (*head).next;
    while (curr_node)
    {
        NODE *tmp_next = (*curr_node).next;
        free(curr_node);
        curr_node = tmp_next;
    }

    free(head);

    return 0;
}

int insertNode(int index, int data, NODE *head, int *list_length)
{   
    if (index > *list_length) {
        printf("fail insertion %d in %d index\n", data, index);
        return 0;
    }

    // 삽입되야 할 노드까지 이동
    NODE *curr_node = head;
    for (int i = 0; i < index; i ++) {
        curr_node = (*curr_node).next;
    }
    
    // 노드 삽입
    NODE *next_node = (*curr_node).next;
    NODE *new_node = malloc(sizeof(NODE));
    (*new_node).data = data;
    (*new_node).next = next_node;
    (*curr_node).next = new_node;
    *list_length += 1;
    printf("success insertion %d in %d index\n", data, index);
}

int searchNode(int value, NODE *head)
{
    NODE *curr_node = (*head).next;
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