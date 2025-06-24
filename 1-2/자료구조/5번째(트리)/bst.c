#include <stdio.h>
#include <stdlib.h>

// 이진 트리 노드 구조체 정의
typedef struct TreeNode {
    int value;
    int height;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// 노드 생성 함수
TreeNode* createNode(int value, int height) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = value;
    node->height = height;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 이진 탐색 트리에 노드 삽입 함수
TreeNode* insert(TreeNode* root, int value, int height) {
    if (root == NULL) {
        return createNode(value, height);
    }
    if (value < root->value) {
        root->left = insert(root->left, value, height + 1);
    } else {
        root->right = insert(root->right, value, height + 1);
    }
    return root;
}

// 왼쪽 → 오른쪽 순서로 리프 노드 출력
void printLeaves(TreeNode* root) {
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL) {
        printf("%d %d\n", root->value, root->height);
        return;
    }
    printLeaves(root->left);
    printLeaves(root->right);
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int n;
    scanf("%d", &n);

    TreeNode* root = NULL;
    for (int i = 0; i < n; ++i) {
        int val;
        scanf("%d", &val);
        root = insert(root, val, 1);  // 루트 높이는 1
    }

    printLeaves(root);
    freeTree(root);
    return 0;
}

