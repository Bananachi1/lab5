#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) { fprintf(stderr, "Ошибка памяти\n"); exit(EXIT_FAILURE); }
    node->data  = value;
    node->left  = NULL;
    node->right = NULL;
    return node;
}

Node *insert(Node *root, int value) {
    if (root == NULL)            return createNode(value);
    if (value < root->data)      root->left  = insert(root->left,  value);
    else if (value > root->data) root->right = insert(root->right, value);
    else printf(" Значение %d уже есть в дереве, пропущено.\n", value);
    return root;
}

void preorder(Node *root) {
    if (!root) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void postorder(Node *root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

void dfs(Node *root) { inorder(root); }

#define MAX_Q 1024
void bfs(Node *root) {
    if (!root) return;
    Node *q[MAX_Q];
    int front = 0, rear = 0;
    q[rear++] = root;
    while (front < rear) {
        Node *cur = q[front++];
        printf("%d ", cur->data);
        if (cur->left)  q[rear++] = cur->left;
        if (cur->right) q[rear++] = cur->right;
    }
}

Node *search(Node *root, int key) {
    if (!root || root->data == key) return root;
    return (key < root->data)
        ? search(root->left,  key)
        : search(root->right, key);
}

int height(Node *root) {
    if (!root) return 0;
    int l = height(root->left), r = height(root->right);
    return 1 + (l > r ? l : r);
}

void freeTree(Node *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* ── Меню ── */
void printMenu(void) {
    printf("\n================================\n");
    printf("        МЕНЮ  (BST)               \n");
    printf("  1. Вставить элемент             \n");
    printf("  2. Показать все обходы          \n");
    printf("  3. Найти элемент                \n");
    printf("  4. Показать высоту дерева       \n");
    printf("  0. Выход                        \n");
    printf("\n================================\n");
    printf("Ваш выбор: ");
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Node *root = NULL;
    int choice, value;

    printf("=== Бинарное дерево поиска ===\n");

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }

        switch (choice) {

        case 1:
            printf("Введите целое число: ");
            if (scanf("%d", &value) == 1) {
                root = insert(root, value);
                printf("  %d добавлен в дерево.\n", value);
            } else {
                printf("  Некорректный ввод.\n");
                while (getchar() != '\n');
            }
            break;

        case 2:
            if (!root) { printf("  [!] Дерево пустое.\n"); break; }
            printf("\nПрямой    (Preorder):  "); preorder(root);  printf("\n");
            printf("Центриров.(Inorder):   "); inorder(root);   printf("\n");
            printf("Обратный  (Postorder): "); postorder(root); printf("\n");
            printf("DFS (рекурс.):          "); dfs(root);       printf("\n");
            printf("BFS (обход в ширину):        "); bfs(root);       printf("\n");
            break;

        case 3:
            if (!root) { printf("  [!] Дерево пустое.\n"); break; }
            printf("Введите ключ для поиска: ");
            if (scanf("%d", &value) == 1) {
                Node *found = search(root, value);
                if (found) printf("  Элемент %d найден в дереве.\n", found->data);
                else       printf("  Элемент %d не найден.\n", value);
            }
            break;

        case 4:
            if (!root) { printf(" Дерево пустое.\n"); break; }
            printf(" Высота дерева: %d\n", height(root));
            break;

        case 0:
            printf("Выход...\n");
            break;

        default:
            printf(" Неверный выбор. Попробуйте снова.\n");
        }

    } while (choice != 0);

    freeTree(root);
    return 0;
}