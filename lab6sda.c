#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (!root) return createNode(value);
    if (value < root->data)      root->left  = insert(root->left,  value);
    else if (value > root->data) root->right = insert(root->right, value);
    else printf("  [!] Значение %d уже есть в дереве.\n", value);
    return root;
}

void inorder(Node *root) {
    if (!root) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

void preorder(Node *root) {
    if (!root) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

#define MAX_Q 1024
void bfs(Node *root) {
    if (!root) return;
    Node *q[MAX_Q]; int front = 0, rear = 0;
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

Node *findMin(Node *root) {
    if (!root) return NULL;
    while (root->left) root = root->left;
    return root;
}

Node *findMax(Node *root) {
    if (!root) return NULL;
    while (root->right) root = root->right;
    return root;
}

int height(Node *root) {
    if (!root) return 0;
    int l = height(root->left), r = height(root->right);
    return 1 + (l > r ? l : r);
}

Node *deleteNode(Node *root, int key) {
    if (!root) {
        printf("   Элемент %d не найден в дереве.\n", key);
        return NULL;
    }

    if (key < root->data) {
        root->left  = deleteNode(root->left,  key);
    } else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } else {

        if (!root->left && !root->right) {
            free(root);
            printf("   Удалён лист.\n");
            return NULL;
        }

        if (!root->left) {
            Node *tmp = root->right;
            free(root);
            printf("   Удалён узел с одним правым потомком.\n");
            return tmp;
        }
        if (!root->right) {
            Node *tmp = root->left;
            free(root);
            printf("   Удалён узел с одним левым потомком.\n");
            return tmp;
        }

        Node *successor = findMin(root->right);
        printf("  Удалён узел с двумя потомками. Замена: %d\n", successor->data);
        root->data  = successor->data;
        root->right = deleteNode(root->right, successor->data); 
    }
    return root;
}

Node *freeTree(Node *root) {
    if (!root) return NULL;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return NULL;
}

void saveToFile(Node *root, FILE *f) {
    if (!root) {
        fprintf(f, "-1 ");
        return;
    }
    fprintf(f, "%d ", root->data);
    saveToFile(root->left,  f);
    saveToFile(root->right, f);
}

int saveTree(Node *root, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { printf("   Не удалось открыть файл для записи.\n"); return 0; }
    saveToFile(root, f);
    fclose(f);
    printf("   Дерево сохранено в файл: %s\n", filename);
    return 1;
}

Node *loadFromFile(FILE *f) {
    int val;
    if (fscanf(f, "%d", &val) != 1) return NULL;
    if (val == -1) return NULL;
    Node *node  = createNode(val);
    node->left  = loadFromFile(f);
    node->right = loadFromFile(f);
    return node;
}

Node *loadTree(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("   Файл не найден: %s\n", filename); return NULL; }
    Node *root = loadFromFile(f);
    fclose(f);
    printf("   Дерево загружено из файла: %s\n", filename);
    return root;
}

void printMenu(void) {
    printf("\n===========================================\n");
    printf("          МЕНЮ  —  BST  Лаб. №6            \n");
    printf("   1. Вставить элемент                     \n");
    printf("   2. Удалить элемент                      \n");
    printf("   3. Найти элемент                        \n");
    printf("   4. Минимальный / Максимальный элемент   \n");
    printf("   5. Высота дерева                        \n");
    printf("   6. Все обходы                           \n");
    printf("   7. Сохранить дерево в файл              \n");
    printf("   8. Загрузить дерево из файла            \n");
    printf("   9. Очистить дерево (удалить всё)        \n");
    printf("   0. Выход                                \n");
    printf("===========================================\n");
    printf("Ваш выбор: ");
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Node *root = NULL;
    int choice, value;
    char filename[256];

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }

        switch (choice) {

        case 1:
            printf("Введите число: ");
            if (scanf("%d", &value) == 1) {
                root = insert(root, value);
                printf("  %d добавлен.\n", value);
            }
            break;

        case 2:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("Введите число для удаления: ");
            if (scanf("%d", &value) == 1)
                root = deleteNode(root, value);
            break;

        case 3:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("Введите ключ для поиска: ");
            if (scanf("%d", &value) == 1) {
                Node *found = search(root, value);
                if (found) printf("   Элемент %d найден.\n", found->data);
                else       printf("   Элемент %d не найден.\n", value);
            }
            break;

        case 4:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("  Минимальный элемент: %d\n", findMin(root)->data);
            printf("  Максимальный элемент: %d\n", findMax(root)->data);
            break;

        case 5:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("  Высота дерева: %d\n", height(root));
            break;

        case 6:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("  Preorder  (NLR): "); preorder(root);  printf("\n");
            printf("  Inorder   (LNR): "); inorder(root);   printf("\n");
            printf("  Postorder (LRN): "); postorder(root); printf("\n");
            printf("  BFS (уровни):    "); bfs(root);       printf("\n");
            break;

        case 7:
            if (!root) { printf("   Дерево пустое.\n"); break; }
            printf("Введите имя файла: ");
            scanf("%255s", filename);
            saveTree(root, filename);
            break;

        case 8:
            printf("Введите имя файла: ");
            scanf("%255s", filename);
            root = freeTree(root);
            root = loadTree(filename);
            break;

        case 9:
            root = freeTree(root);
            printf("   Дерево полностью удалено из памяти.\n");
            break;

        case 0:
            root = freeTree(root);
            printf("Память освобождена. Выход.\n");
            break;

        default:
            printf("   Неверный выбор.\n");
        }

    } while (choice != 0);

    return 0;
}