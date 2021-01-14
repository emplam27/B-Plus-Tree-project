#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MIN_DEGREE 2
#define MAX_KEY (MIN_DEGREE*2 - 1)
#define MIN_KEY (MIN_DEGREE - 1)

typedef struct _node {
	bool is_leaf;
	int key_count;
	int keys[MAX_KEY + 2];
	struct _node *pointers[MAX_KEY + 3];
	struct _node *parent, *left, *right;
} node;

node* node_create();
void b_plus_tree_create(node** root);
void b_plus_tree_insert(node** root, int k, int v);
void b_plus_tree_delete(node* sub_root, node** root, int k);
void node_insert(node* parent, int k, int v);
void node_split(node* new_root, int index);
void node_delete(node* sub_root, int k);
void move_leaf_key_right_to_left(node* left_child, node* right_child, int* parent_key);
void move_key_right_to_left(node* left_child, node* right_child, int* parent_key);
void move_leaf_key_left_to_right(node* left_child, node* right_child, int* parent_key);
void move_key_left_to_right(node* left_child, node* right_child, int* parent_key);
void bind_node(node* parent, node* left_child, node* right_child, int index);
void bind_leaf_node(node* parent, node* left_child, node* right_child, int index);
void display(node* root, int depth);
void test_case(node** root, int size);
void print_for_exam(node* cur);


int main() {
	node* root;
	b_plus_tree_create(&root);

	test_case(&root, 1000);
	return 0;
}

node* node_create() {
	node* new_node = (node*)malloc(sizeof(node));
	if (new_node == NULL) {
		perror("Record Creation Error");
		exit(EXIT_FAILURE);
	}
	new_node->key_count = 0;
	new_node->is_leaf = true;
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}

void b_plus_tree_create(node** root) {
	node *new_node = node_create();
	if (new_node == NULL) {
		perror("Record creation.");
		exit(EXIT_FAILURE);
	}
	*root = new_node;
}

void b_plus_tree_insert(node** root, int k, int v) {
	node* curr_root = *root;
	if (curr_root->key_count == MAX_KEY) {
		node* new_root = node_create();
		*root = new_root;
		new_root->is_leaf = false;
		new_root->pointers[1] = curr_root;
		curr_root->parent = new_root;
		node_split(new_root, 1);
		node_insert(new_root, k, v);
	}
	else {
		node_insert(curr_root, k, v);
	}
}

void node_insert(node* sub_root, int k, int v) {
	int* value = (int*)malloc(sizeof(int));
	if (value == NULL) {
		perror("Value Creation Error");
		exit(EXIT_FAILURE);
	}
	*value = v;
	if (!sub_root->is_leaf) {
		int i = 1;
		while (i <= sub_root->key_count && k > sub_root->keys[i]) {
			i++;
		}
		if (sub_root->pointers[i]->key_count == MAX_KEY) {
			node_split(sub_root, i);
			if (sub_root->keys[i] <= k) {
				i ++;
			}
		}
		node_insert(sub_root->pointers[i], k, v);
		return;
	}
	else {
		int i = sub_root->key_count;
		while (i >= 1 && k < sub_root->keys[i]) {
			sub_root->keys[i + 1] = sub_root->keys[i];
			sub_root->pointers[i + 1] = sub_root->pointers[i];
			i--;
		}
		sub_root->keys[i + 1] = k;
		sub_root->pointers[i + 1] = (void *)value;
		sub_root->key_count += 1;
	}
	return;
}

void node_split(node* parent, int index) {
	node* left_child = parent->pointers[index];
	node* right_child = node_create();
	right_child->is_leaf = left_child->is_leaf;
	right_child->parent = parent;

	if (!left_child->is_leaf) {
		for (int i = parent->key_count; i >= index; i--) {
			parent->keys[i + 1] = parent->keys[i];
		}
		for (int i = (parent->key_count) + 1; i >= index; i--) {
			parent->pointers[i + 1] = parent->pointers[i];
		}
		parent->key_count += 1;
		parent->keys[index] = left_child->keys[MIN_KEY + 1];
		parent->pointers[index + 1] = right_child;

		left_child->key_count = MIN_KEY;
		right_child->key_count = MIN_KEY;
		for (int i = 1; i <= MIN_KEY; i++) {
			right_child->keys[i] = left_child->keys[MIN_KEY + 1 + i];
		}
		for (int i = 1; i <= MIN_KEY + 1; i++) {
			right_child->pointers[i] = left_child->pointers[MIN_KEY + 1 + i];
		}
	}

	else {
		for (int i = parent->key_count; i >= index; i--) {
			parent->keys[i + 1] = parent->keys[i];
		}
		for (int i = (parent->key_count) + 1; i >= index + 1; i--) {
			parent->pointers[i + 1] = parent->pointers[i];
		}
		parent->key_count += 1;
		parent->keys[index] = left_child->keys[MIN_KEY];
		parent->pointers[index + 1] = right_child;
		left_child->key_count = MIN_KEY;
		right_child->key_count = MIN_KEY + 1;
		for (int i = 1; i <= MIN_KEY + 1; i++) {
			right_child->keys[i] = left_child->keys[MIN_KEY + i];
		}
		for (int i = 1; i <= MIN_KEY + 1; i++) {
			right_child->pointers[i] = left_child->pointers[MIN_KEY + i];
		}
	}
	return;
}

void b_plus_tree_delete(node* sub_root, node** root, int k) {
	if ((*root)->key_count == 0) {
		if ((*root)->is_leaf) {
			printf("tree is empty\n");
			return;
		}
	}
	node_delete(sub_root, k);
	if ((*root)->key_count == 0) {
		if ((*root)->is_leaf) {
			printf("tree is empty\n");
			free(*root);
			b_plus_tree_create(root);
			return;
		}
		else {
			node* old_root = *root;
			*root = (*root)->pointers[1];
			free(old_root);
			return;
		}
	}
}

void node_delete(node* sub_root, int k) {
	// 리프
	if (sub_root->is_leaf) {
		int origin_key_count = sub_root->key_count;
		for (int i = 1; i <= sub_root->key_count; i++) {
			if (sub_root->keys[i] == k) {
				free(sub_root->pointers[i]);
				for (int j = i; j < sub_root->key_count; j++) {
					sub_root->keys[j] = sub_root->keys[j + 1];
					sub_root->pointers[j] = sub_root->pointers[j + 1];
				}
				sub_root->key_count -= 1;
				break;
			}
		}
		if (origin_key_count == sub_root->key_count) {
			printf("%d not in b-tree\n", k);
		}
		return;
	}
	// not 리프, 본인보다 크거나 같은 키 만나면 정지
	int i = 1;
	while (sub_root->keys[i] < k && i <= sub_root->key_count) {
		i += 1;
	}
	// 가장 오른쪽 키 검사일 때
	if (i == sub_root->key_count + 1) {
		node* left_child = sub_root->pointers[i - 1];
		node* right_child = sub_root->pointers[i];
		// 오른쪽검사
		if (right_child->key_count > MIN_KEY) {
			node_delete(right_child, k);
			return;
		}
		// 오른쪽 키 개수가 부족하면 왼쪽에서 가져오기
		if (left_child->key_count > MIN_KEY) {
			if (!right_child->is_leaf) {
				move_key_left_to_right(left_child, right_child, &(sub_root->keys[i - 1]));
			}
			else {
				move_leaf_key_left_to_right(left_child, right_child, &(sub_root->keys[i - 1]));
			}
			node_delete(right_child, k);
			return;
		}
		// 둘다 부족하면 병합
		else {
			if (!left_child->is_leaf) {
				bind_node(sub_root, left_child, right_child, i - 1);
			}
			else {
				bind_leaf_node(sub_root, left_child, right_child, i - 1);
			}
			node_delete(left_child, k);
			return;
		}
		return;
	}
	// 가장 오른쪽 키가 아닌 검사일 때 
	else {
		node* left_child = sub_root->pointers[i];
		node* right_child = sub_root->pointers[i + 1];
		// 왼쪽검사
		if (left_child->key_count > MIN_KEY) {
			node_delete(left_child, k);
			return;
		}
		// 왼쪽 키 개수가 부족하면 오른쪽에서 가져오기
		if (right_child->key_count > MIN_KEY) {
			if (!right_child->is_leaf) {
				move_key_right_to_left(left_child, right_child, &(sub_root->keys[i]));
			}
			else {
				move_leaf_key_right_to_left(left_child, right_child, &(sub_root->keys[i]));
			}
			node_delete(left_child, k);
			return;
		}
		// 둘다 부족하면 병합
		else {
			if (!left_child->is_leaf) {
				bind_node(sub_root, left_child, right_child, i);
			}
			else {
				bind_leaf_node(sub_root, left_child, right_child, i);
			}
			node_delete(left_child, k);
			return;
		}
		return;
	}
}

void move_leaf_key_right_to_left(node* left_child, node* right_child, int* parent_key) {
	*parent_key = right_child->keys[1];
	left_child->keys[left_child->key_count + 1] = right_child->keys[1];
	left_child->pointers[left_child->key_count + 1] = right_child->pointers[1];
	left_child->key_count += 1;

	for (int j = 1; j <= right_child->key_count - 1; j++) {
		right_child->keys[j] = right_child->keys[j + 1];
		right_child->pointers[j] = right_child->pointers[j + 1];
	}
	right_child->key_count -= 1;
}

void move_key_right_to_left(node* left_child, node* right_child, int* parent_key) {
	left_child->keys[(left_child->key_count) + 1] = *parent_key;
	left_child->pointers[(left_child->key_count) + 2] = right_child->pointers[1];
	left_child->pointers[(left_child->key_count) + 2]->parent = left_child;
	left_child->key_count += 1;
	*parent_key = right_child->keys[1];
	for (int j = 1; j <= right_child->key_count - 1; j++) {
		right_child->keys[j] = right_child->keys[j + 1];
		right_child->pointers[j] = right_child->pointers[j + 1];
	}
	right_child->pointers[right_child->key_count] = right_child->pointers[right_child->key_count + 1];
	right_child->key_count -= 1;
}

void move_leaf_key_left_to_right(node* left_child, node* right_child, int* parent_key) {
	for (int j = right_child->key_count; j >= 1; j--) {
		right_child->keys[j + 1] = right_child->keys[j];
		right_child->pointers[j + 1] = right_child->pointers[j];
	}
	right_child->keys[1] = left_child->keys[left_child->key_count];
	right_child->pointers[1] = left_child->pointers[left_child->key_count];
	right_child->key_count += 1;
	left_child->key_count -= 1;
	*parent_key = left_child->keys[left_child->key_count];
}

void move_key_left_to_right(node* left_child, node* right_child, int* parent_key) {
	right_child->pointers[(right_child->key_count) + 2] = right_child->pointers[right_child->key_count + 1];
	for (int j = right_child->key_count; j >= 1; j--) {
		right_child->keys[j + 1] = right_child->keys[j];
		right_child->pointers[j + 1] = right_child->pointers[j];
	}
	right_child->keys[1] = *parent_key;
	right_child->pointers[1] = left_child->pointers[left_child->key_count + 1];
	right_child->pointers[1]->parent = right_child;
	right_child->key_count += 1;
	*parent_key = left_child->keys[left_child->key_count];
	left_child->key_count -= 1;
}

void bind_node(node* parent, node* left_child, node* right_child, int index) {
	left_child->keys[left_child->key_count + 1] = parent->keys[index];
	for (int j = index; j < parent->key_count; j++) {
		parent->keys[j] = parent->keys[j + 1];
	}
	for (int j = index + 1; j <= parent->key_count; j++) {
		parent->pointers[j] = parent->pointers[j + 1];
	}
	parent->key_count -= 1;
	for (int j = 1; j <= right_child->key_count; j++) {
		left_child->keys[(MIN_KEY + 1) + j] = right_child->keys[j];
	}
	for (int j = 1; j <= (right_child->key_count) + 1; j++) {
		left_child->pointers[(MIN_KEY + 1) + j] = right_child->pointers[j];
	}
	left_child->key_count += (right_child->key_count + 1);
	free(right_child);
}

void bind_leaf_node(node* parent, node* left_child, node* right_child, int index) {
	for (int j = index; j < parent->key_count; j++) {
		parent->keys[j] = parent->keys[j + 1];
	}
	for (int j = index + 1; j <= parent->key_count; j++) {
		parent->pointers[j] = parent->pointers[j + 1];
	}
	parent->key_count -= 1;
	for (int j = 1; j <= right_child->key_count; j++) {
		left_child->keys[MIN_KEY + j] = right_child->keys[j];
		left_child->pointers[MIN_KEY + j] = right_child->pointers[j];
	}
	left_child->key_count += right_child->key_count;
	free(right_child);
}

void test_case(node** root, int size) {
	int* out_arr = (int*)malloc(sizeof(int) * 1000000);
	if (out_arr == NULL) {
		perror("Test Case creation.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < size; i++) {
		out_arr[i] = i;
	}
	for (int i = 0; i < size; i++)
	{
		int j = i + rand() / (RAND_MAX / (size - i) + 1);
		int t = out_arr[j];
		out_arr[j] = out_arr[i];
		out_arr[i] = t;
	}
	for (int i = 0; i < size; i++) {
		int r = out_arr[i];
		b_plus_tree_insert(root, r, r * 3);
	}
	for (int i = 0; i < size; i++) {
		int r = out_arr[i];
		b_plus_tree_delete(*root, root, r);
	}
}

void display(node* cur_node, int depth) {
	int i;
	if (cur_node->key_count == 0) {
		printf("tree is empty\n");
		return;
	}
	if (cur_node->is_leaf) {
		for (i = 1; i <= cur_node->key_count; i++) {
			for (int j = 1; j <= depth; j++)
				printf("---!");
			printf("%d | ", cur_node->keys[i]);
			int* value = (int*)cur_node->pointers[i];
			printf("%d\n", *value);
		}
		return;
	}
	for (i = 1; i <= cur_node->key_count; i++) {
		display(cur_node->pointers[i], depth + 1);
		for (int j = 1; j <= depth; j++)
			printf("---!");
		printf("%d\n", cur_node->keys[i]);
	}
	display(cur_node->pointers[i], depth + 1);
	return;
}
