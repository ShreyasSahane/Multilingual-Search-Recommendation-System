#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct tri_node tri_node;
typedef struct node {
    unsigned int data;    
    struct node *left;
    struct node *right;
    tri_node *tri_node; \
    int height;
} node;

struct tri_node {
    node *avl_root;     
    bool isEnd;         
};

tri_node *createTriNode() {
    tri_node *t = (tri_node *)malloc(sizeof(tri_node));
    t->avl_root = NULL;
    t->isEnd = false;
    return t;
}

node *createNode(unsigned int data) {
    node *n = (node *)malloc(sizeof(node));
    n->data = data;
    n->left = n->right = NULL;
    n->tri_node = createTriNode();
    n->height = 1;
    return n;
}

int height(node *root) {
    return root ? root->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(node *root) {
    return root ? height(root->left) - height(root->right) : 0;
}

node *rightRotate(node *y) {
    node *x = y->left;
    node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

node *leftRotate(node *x) {
    node *y = x->right;
    node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

typedef struct AVLInsert {
    node *root;
    node *end_n;
} AVLInsert;

AVLInsert *createAVLInsert() {
    AVLInsert *AI = (AVLInsert *)malloc(sizeof(AVLInsert));
    AI->root = NULL;
    AI->end_n = NULL;
    return AI;
}

AVLInsert *insertNode(node *root, unsigned int data) {
    AVLInsert *AI = createAVLInsert();
    AI->root = root;
    if (AI->root == NULL) {
        AI->root = createNode(data);
        AI->end_n = AI->root;
        return AI;
    }
    if (data < AI->root->data) {
        AVLInsert *leftAI = insertNode(AI->root->left, data);
        AI->root->left = leftAI->root;
        AI->end_n = leftAI->end_n;
        free(leftAI);
    }
    else if (data > AI->root->data) {
        AVLInsert *rightAI = insertNode(AI->root->right, data);
        AI->root->right = rightAI->root;
        AI->end_n = rightAI->end_n;
        free(rightAI);
    }
    else {
        AI->end_n = AI->root; 
        return AI;
    }
    AI->root->height = 1 + max(height(AI->root->left), height(AI->root->right));
    int balance = getBalance(AI->root);
    if (balance > 1 && data < AI->root->left->data)
        AI->root = rightRotate(AI->root);
    else if (balance < -1 && data > AI->root->right->data)
        AI->root = leftRotate(AI->root);
    else if (balance > 1 && data > AI->root->left->data) {
        AI->root->left = leftRotate(AI->root->left);
        AI->root = rightRotate(AI->root);
    }
    else if (balance < -1 && data < AI->root->right->data) {
        AI->root->right = rightRotate(AI->root->right);
        AI->root = leftRotate(AI->root);
    }
    return AI;
}
tri_node *insert_tri_node(tri_node *root, const unsigned int *codepoints, int length) {
    tri_node *curr = root;
    for (int i = 0; i < length; i++) {
        AVLInsert *AI = insertNode(curr->avl_root, codepoints[i]);
        curr->avl_root = AI->root;
        curr = AI->end_n->tri_node;
        free(AI);
    }
    curr->isEnd = true;
    return root;
}

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror(filename);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    char *buffer = malloc(size + 1);
    if (!buffer) { fclose(fp); return NULL; }
    size_t read_size = fread(buffer, 1, size, fp);
    buffer[read_size] = '\0';
    fclose(fp);
    return buffer;
}

int decode_utf8(const char *s, unsigned int *codepoints, int max) {
    int count = 0, pos = 0;
    while (s[pos] && count < max) {
        unsigned char c = s[pos];
        if (c <= 0x7F) {
            codepoints[count++] = c;
            pos++;
        }
        else if ((c & 0xE0) == 0xC0) {
            if (!s[pos+1]) break;
            unsigned int code = ((c & 0x1F) << 6) | (s[pos+1] & 0x3F);
            codepoints[count++] = code;
            pos += 2;
        }
        else if ((c & 0xF0) == 0xE0) {
            if (!s[pos+1] || !s[pos+2]) break;
            unsigned int code = ((c & 0x0F) << 12) |
                                (((unsigned char)s[pos+1] & 0x3F) << 6) |
                                (((unsigned char)s[pos+2] & 0x3F));
            codepoints[count++] = code;
            pos += 3;
        }
        else {
            codepoints[count++] = 0xFFFD;
            pos++;
        }
    }
    return count;
}
void serialize_avl_preorder(node *root, FILE *f);
void serialize_trie_preorder(tri_node *t, FILE *f);
void serialize_avl_inorder(node *root, FILE *f);
void serialize_trie_inorder(tri_node *t, FILE *f);

void serialize_avl_preorder(node *root, FILE *f) {
    if (!root) { 
        fprintf(f, "X "); 
        return; 
    }
    fprintf(f, "N %u ", root->data);
    serialize_trie_preorder(root->tri_node, f);
    serialize_avl_preorder(root->left, f);
    serialize_avl_preorder(root->right, f);
}

void serialize_trie_preorder(tri_node *t, FILE *f) {
    if (!t) { 
        fprintf(f, "X "); 
        return; 
    }
    fprintf(f, "T %d ", t->isEnd ? 1 : 0);
    serialize_avl_preorder(t->avl_root, f);
}

void serialize_avl_inorder(node *root, FILE *f) {
    if (!root) { 
        fprintf(f, "X "); 
        return; 
    }
    serialize_avl_inorder(root->left, f);
    fprintf(f, "N %u ", root->data);
    serialize_trie_inorder(root->tri_node, f);
    serialize_avl_inorder(root->right, f);
}

void serialize_trie_inorder(tri_node *t, FILE *f) {
    if (!t) { 
        fprintf(f, "X "); 
        return; 
    }
    fprintf(f, "T %d ", t->isEnd ? 1 : 0);
    serialize_avl_inorder(t->avl_root, f);
}

int main() {
    char *text = read_file("hi.txt");
    if (!text) {
        fprintf(stderr, "Error reading hi.txt\n");
        return 1;
    }

    tri_node *trie_root = createTriNode();

    char *token = strtok(text, " \n\t\r");
    while (token != NULL) {
        int tokenLen = strlen(token);
        unsigned int *codepoints = malloc((tokenLen + 1) * sizeof(unsigned int));
        int count = decode_utf8(token, codepoints, tokenLen + 1);
        trie_root = insert_tri_node(trie_root, codepoints, count);
        free(codepoints);
        token = strtok(NULL, " \n\t\r");
    }
    free(text);
    FILE *fp_pre = fopen("preorder.txt", "w");
    if (!fp_pre) { perror("preorder.txt"); return 1; }
    FILE *fp_in = fopen("inorder.txt", "w");
    if (!fp_in) { perror("inorder.txt"); return 1; }

    serialize_trie_preorder(trie_root, fp_pre);
    serialize_trie_inorder(trie_root, fp_in);
    
    fclose(fp_pre);
    fclose(fp_in);
    
    printf("Trie serialized into preorder.txt and inorder.txt successfully.\n");
    return 0;
}
