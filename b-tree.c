#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_DEGREE 2
#define MAX_KEY (MIN_DEGREE * 2 - 1)
#define MIN_KEY (MIN_DEGREE - 1)
#define MAX_CHILD (MIN_DEGREE * 2)
#define MIN_CHILD (MIN_DEGREE)

typedef struct _node {
    bool is_leaf;
    int key[MAX_KEY + 1], key_count;
    struct _node *child[MAX_KEY + 1];
} node;


void b_tree_create(node **root);
void b_tree_insert(node **root, int k);
void b_tree_insert_nonfull(node *sub_root, int k);
void b_tree_search(node *sub_root, int k);
void b_tree_delete(node *sub_root, int k);
void split_node(node *parent_node, int child_index);
int display(node *node, int blanks);


int main() {
    node *root;
    b_tree_create(&root);
    b_tree_insert(&root, 1);
    b_tree_insert(&root, 2);
    b_tree_insert(&root, 3);
    b_tree_insert(&root, 4);
    b_tree_insert(&root, 5);
    b_tree_insert(&root, 6);
    b_tree_insert(&root, 7);
    b_tree_insert(&root, 8);
    b_tree_insert(&root, 9);
    b_tree_insert(&root, 10);
    b_tree_insert(&root, 11);
    b_tree_insert(&root, 12);
    b_tree_insert(&root, 13);
    b_tree_insert(&root, 14);
    b_tree_insert(&root, 15);
    b_tree_insert(&root, 16);
    b_tree_insert(&root, 17);
    b_tree_insert(&root, 18);
    display(root, 0);
    b_tree_search(root, 18);
}

// 루트노드 생성
void b_tree_create(node **root) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->is_leaf = true;
    new_node->key_count = 0;
    *root = new_node;
}

void split_node(node *parent_node, int child_index) {
    // 새로운 자식 노드를 만들어 중앙값 이후의 값들을 넘겨준다.
    node *right_child_node = (node*)malloc(sizeof(node));
    node *left_child_node = parent_node->child[child_index];
    right_child_node->is_leaf = left_child_node->is_leaf;
    right_child_node->key_count = MIN_KEY;
    for (int i = 1; i <= MIN_KEY; i++) {
        right_child_node->key[i] = left_child_node->key[i + MIN_DEGREE];
    }
    if (!left_child_node->is_leaf) {
        for (int i = 1; i <= MIN_DEGREE; i++) {
            right_child_node->child[i] = left_child_node->child[i + MIN_DEGREE];
        }
    }
    left_child_node->key_count = MIN_KEY;

    // 부모의 키, 자식을 한칸씩 뒤로 밀어준다.
    for (int i = parent_node->key_count + 1; i >= child_index + 1; i--) {
        parent_node->child[i + 1] = parent_node->child[i];
    }
    parent_node->child[child_index + 1] = right_child_node;
    for (int i = parent_node->key_count; i >= child_index; i--) {
        parent_node->key[i + 1] = parent_node->key[i];
    }
    // 부모의 중앙값 넣어주기
    parent_node->key[child_index] = left_child_node->key[MIN_DEGREE];
    parent_node->key_count += 1;
}


void b_tree_insert(node **root, int k) {
    node *curr_root = *root;
    if ((*root)->key_count == MAX_KEY) {
        node *new_root = (node*)malloc(sizeof(node));
        *root = new_root;
        new_root->is_leaf = false;
        new_root->key_count = 0;
        new_root->child[1] = curr_root;
        split_node(new_root, 1);
        b_tree_insert_nonfull(new_root, k);
    }
    else {
        b_tree_insert_nonfull(curr_root, k);
    }
}

void b_tree_insert_nonfull(node *sub_root, int k) {
    int i = sub_root->key_count;
    if (sub_root->is_leaf) {
        while (i >= 1 && k < sub_root->key[i]){
            sub_root->key[i + 1] = sub_root->key[i];
            i--;
        }
        sub_root->key[i + 1] = k;
        sub_root->key_count++;
    }
    else {
        while (i >= 1 && k < sub_root->key[i]) {
            i--;
        }
        i++;
        if (sub_root->child[i]->key_count == MAX_KEY) {
            split_node(sub_root, i);
            if (k > sub_root->key[i]) {
                i++;
            }
        }
        b_tree_insert_nonfull(sub_root->child[i], k);
    }
}

void b_tree_search(node *sub_root, int k) {
    int i = 1;
    while (i <= sub_root->key_count && k > sub_root->key[i]) {
        i++;
    }
    if (i <= sub_root->key_count && k == sub_root->key[i]) {
        printf("success find %d\n", k);
    }
    else if (sub_root->is_leaf) {
        printf("fail find %d\n", k);
    }
    else {
        b_tree_search(sub_root->child[i], k);
    }
}

// void b_tree_delete(node *sub_root, int k) {
//     if (sub_root->is_leaf) {
//         for (int i = 1; i <= sub_root->key_count; i++) {
//             if (sub_root->key[i] == k) {
//                 for (int j = i; j < sub_root->key_count; j++) {
//                     sub_root->key[j] = sub_root->key[j + 1];
//                 }
//                 sub_root->key_count--;
//             }
//         }
//     }
// }

int display(node *cur_node, int blanks) {
    int i;
    if (cur_node->is_leaf) {

        for (i = 1; i <= cur_node->key_count; i++) {
            for (int j = 1; j <= blanks; j ++)
                printf("---!");
            printf("%d\n", cur_node->key[i]);
        }
        return 0;
    }

    for (i = 1; i <= cur_node->key_count; i++) {
        display(cur_node->child[i], blanks + 1);
        for (int j = 1; j <= blanks; j ++)
            printf("---!");
        printf("%d\n", cur_node->key[i]);
    }
    display(cur_node->child[i], blanks + 1);
    return 0;
}