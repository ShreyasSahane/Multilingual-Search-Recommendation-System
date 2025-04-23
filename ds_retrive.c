#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct tri_node tri_node;
typedef struct node {
    unsigned int data;      
    struct node *left;
    struct node *right;
    tri_node *tri_node;    
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

node *deserialize_avl_preorder(char **tokens, int *index);
tri_node *deserialize_trie_preorder(char **tokens, int *index);

node *deserialize_avl_preorder(char **tokens, int *index) {
    if (strcmp(tokens[*index], "X") == 0) {
        (*index)++;
        return NULL;
    }
    if (strcmp(tokens[*index], "N") != 0) {
        fprintf(stderr, "Error in token stream at token %d: expected 'N', got '%s'\n", *index, tokens[*index]);
        exit(1);
    }
    (*index)++; 
    unsigned int data = (unsigned int)atoi(tokens[*index]);
    (*index)++;
    node *root = createNode(data);
    root->tri_node = deserialize_trie_preorder(tokens, index);
    root->left = deserialize_avl_preorder(tokens, index);
    root->right = deserialize_avl_preorder(tokens, index);
    root->height = 1 + max(height(root->left), height(root->right));
    return root;
}

tri_node *deserialize_trie_preorder(char **tokens, int *index) {
    if (strcmp(tokens[*index], "X") == 0) {
        (*index)++;
        return NULL;
    }
    if (strcmp(tokens[*index], "T") != 0) {
        fprintf(stderr, "Error in token stream at token %d: expected 'T', got '%s'\n", *index, tokens[*index]);
        exit(1);
    }
    (*index)++;
    int isEnd = atoi(tokens[*index]);
    (*index)++;
    tri_node *t = createTriNode();
    t->isEnd = (isEnd == 1);
    t->avl_root = deserialize_avl_preorder(tokens, index);
    return t;
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

char **tokenize(const char *str, int *token_count) {
    char *copy = strdup(str);
    int capacity = 100;
    char **tokens = malloc(capacity * sizeof(char *));
    int count = 0;
    char *token = strtok(copy, " \n\t\r");
    while (token) {
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(char *));
        }
        tokens[count++] = strdup(token);
        token = strtok(NULL, " \n\t\r");
    }
    *token_count = count;
    free(copy);
    return tokens;
}

void free_tokens(char **tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

struct Mapping {
    char *eng;
    unsigned int *codes;
    int count;
    int dependent; 
};

struct Mapping mappings[] = {
    // independent Vowels
    {"a", (unsigned int[]){0x0905}, 1, 0},
    {"A", (unsigned int[]){0x0906}, 1, 0},
    {"aa", (unsigned int[]){0x0906}, 1, 0},
    {"iI", (unsigned int[]){0x0907}, 1, 0},
    {"II", (unsigned int[]){0x0908}, 1, 0},
    {"iiI", (unsigned int[]){0x0908}, 1, 0},
    {"ee", (unsigned int[]){0x0908}, 1, 0},
    {"u", (unsigned int[]){0x0909}, 1, 0},
    {"U", (unsigned int[]){0x090A}, 1, 0},
    {"uu", (unsigned int[]){0x090A}, 1, 0},
    {"RRi", (unsigned int[]){0x090B}, 1, 0},
    {"R^i", (unsigned int[]){0x090B}, 1, 0},
    {"RRI", (unsigned int[]){0x0960}, 1, 0},
    {"R^I", (unsigned int[]){0x0960}, 1, 0},
    {"LLi", (unsigned int[]){0x090C}, 1, 0},
    {"L^i", (unsigned int[]){0x090C}, 1, 0},
    {"LLI", (unsigned int[]){0x0961}, 1, 0},
    {"L^I", (unsigned int[]){0x0961}, 1, 0},
    {"^e", (unsigned int[]){0x090E}, 1, 0},
    {"e", (unsigned int[]){0x090F}, 1, 0},
    {"E", (unsigned int[]){0x090F}, 1, 0},
    {"ae", (unsigned int[]){0x090D}, 1, 0},
    {"aE", (unsigned int[]){0x090D}, 1, 0},
    {"ai", (unsigned int[]){0x0910}, 1, 0},
    {"^o", (unsigned int[]){0x0912}, 1, 0},
    {"o", (unsigned int[]){0x0913}, 1, 0},
    {"O", (unsigned int[]){0x0913}, 1, 0},
    {"au", (unsigned int[]){0x0914}, 1, 0},
    {"aU", (unsigned int[]){0x0972}, 1, 0},
    {"AU", (unsigned int[]){0x0911}, 1, 0},
    {".a", (unsigned int[]){0x093D}, 1, 0},
    {"OM", (unsigned int[]){0x0950}, 1, 0},
    {"AUM", (unsigned int[]){0x0950}, 1, 0},

    // Consonants
    {"ka", (unsigned int[]){0x0915}, 1, 0},
    {"k", (unsigned int[]){0x0915}, 1, 0},
    {"kha", (unsigned int[]){0x0916}, 1, 0},
    {"kh", (unsigned int[]){0x0916}, 1, 0},
    {"ga", (unsigned int[]){0x0917}, 1, 0},
    {"g", (unsigned int[]){0x0917}, 1, 0},
    {"gha", (unsigned int[]){0x0918}, 1, 0},
    {"gh", (unsigned int[]){0x0918}, 1, 0},
    {"~Na", (unsigned int[]){0x0919}, 1, 0},
    {"~N", (unsigned int[]){0x0919}, 1, 0},

    {"cha", (unsigned int[]){0x091A}, 1, 0},
    {"ch", (unsigned int[]){0x091A}, 1, 0},
    {"Cha", (unsigned int[]){0x091B}, 1, 0},
    {"Ch", (unsigned int[]){0x091B}, 1, 0},
    {"ja", (unsigned int[]){0x091C}, 1, 0},
    {"j", (unsigned int[]){0x091C}, 1, 0},
    {"jha", (unsigned int[]){0x091D}, 1, 0},
    {"jh", (unsigned int[]){0x091D}, 1, 0},
    {"~na", (unsigned int[]){0x091E}, 1, 0},
    {"~n", (unsigned int[]){0x091E}, 1, 0},

    {"Ta", (unsigned int[]){0x091F}, 1, 0},
    {"T", (unsigned int[]){0x091F}, 1, 0},
    {"Tha", (unsigned int[]){0x0920}, 1, 0},
    {"Th", (unsigned int[]){0x0920}, 1, 0},
    {"Da", (unsigned int[]){0x0921}, 1, 0},
    {"D", (unsigned int[]){0x0921}, 1, 0},
    {"Dha", (unsigned int[]){0x0922}, 1, 0},
    {"Dh", (unsigned int[]){0x0922}, 1, 0},
    {"Na", (unsigned int[]){0x0923}, 1, 0},
    {"N", (unsigned int[]){0x0923}, 1, 0},

    {"ta", (unsigned int[]){0x0924}, 1, 0},
    {"t", (unsigned int[]){0x0924}, 1, 0},
    {"tha", (unsigned int[]){0x0925}, 1, 0},
    {"th", (unsigned int[]){0x0925}, 1, 0},
    {"da", (unsigned int[]){0x0926}, 1, 0},
    {"d", (unsigned int[]){0x0926}, 1, 0},
    {"dha", (unsigned int[]){0x0927}, 1, 0},
    {"dh", (unsigned int[]){0x0927}, 1, 0},
    {"na", (unsigned int[]){0x0928}, 1, 0},
    {"n", (unsigned int[]){0x0928}, 1, 0},

    {"pa", (unsigned int[]){0x092A}, 1, 0},
    {"p", (unsigned int[]){0x092A}, 1, 0},
    {"pha", (unsigned int[]){0x092B}, 1, 0},
    {"ph", (unsigned int[]){0x092B}, 1, 0},
    {"ba", (unsigned int[]){0x092C}, 1, 0},
    {"b", (unsigned int[]){0x092C}, 1, 0},
    {"bha", (unsigned int[]){0x092D}, 1, 0},
    {"bh", (unsigned int[]){0x092D}, 1, 0},
    {"ma", (unsigned int[]){0x092E}, 1, 0},
    {"m", (unsigned int[]){0x092E}, 1, 0},

    {"ya", (unsigned int[]){0x092F}, 1, 0},
    {"y", (unsigned int[]){0x092F}, 1, 0},
    {"ra", (unsigned int[]){0x0930}, 1, 0},
    {"r", (unsigned int[]){0x0930}, 1, 0},
    {"la", (unsigned int[]){0x0932}, 1, 0},
    {"l", (unsigned int[]){0x0932}, 1, 0},
    {"va", (unsigned int[]){0x0935}, 1, 0},
    {"v", (unsigned int[]){0x0935}, 1, 0},
    {"sha", (unsigned int[]){0x0936}, 1, 0},
    {"sh", (unsigned int[]){0x0936}, 1, 0},
    {"Sha", (unsigned int[]){0x0937}, 1, 0},
    {"Sh", (unsigned int[]){0x0937}, 1, 0},
    {"sa", (unsigned int[]){0x0938}, 1, 0},
    {"s", (unsigned int[]){0x0938}, 1, 0},
    {"ha", (unsigned int[]){0x0939}, 1, 0},
    {"h", (unsigned int[]){0x0939}, 1, 0},
    {"La", (unsigned int[]){0x0933}, 1, 0},
    {"L", (unsigned int[]){0x0933}, 1, 0},

    // Consonants with Nuqta
    {"qa", (unsigned int[]){0x0958}, 1, 0},
    {"q", (unsigned int[]){0x0958}, 1, 0},
    {"Ka", (unsigned int[]){0x0959}, 1, 0},
    {"K", (unsigned int[]){0x0959}, 1, 0},
    {"Ga", (unsigned int[]){0x095A}, 1, 0},
    {"G", (unsigned int[]){0x095A}, 1, 0},
    {"za", (unsigned int[]){0x095B}, 1, 0},
    {"z", (unsigned int[]){0x095B}, 1, 0},
    {"fa", (unsigned int[]){0x095C}, 1, 0},
    {"f", (unsigned int[]){0x095C}, 1, 0},
    {".Da", (unsigned int[]){0x095D}, 1, 0},
    {".D", (unsigned int[]){0x095D}, 1, 0},
    {"Ra", (unsigned int[]){0x095D}, 1, 0},
    {"R", (unsigned int[]){0x095D}, 1, 0},
    {".Dha", (unsigned int[]){0x095E}, 1, 0},
    {".Dh", (unsigned int[]){0x095E}, 1, 0},
    {"Rha", (unsigned int[]){0x095E}, 1, 0},
    {"Rh", (unsigned int[]){0x095E}, 1, 0},
    {"wa", (unsigned int[]){0x0935, 0x093C}, 2, 0},
    {"w", (unsigned int[]){0x0935, 0x093C}, 2, 0},

    // Irregular Consonant
    {"kSa", (unsigned int[]){0x0915, 0x094D, 0x0937}, 3, 0},
    {"kSha", (unsigned int[]){0x0915, 0x094D, 0x0937}, 3, 0},
    {"xa", (unsigned int[]){0x0915, 0x094D, 0x0937}, 3, 0},
    {"tra", (unsigned int[]){0x0924, 0x094D, 0x0930}, 3, 0},
    {"tr", (unsigned int[]){0x0924, 0x094D, 0x0930}, 3, 0},
    {"GYa", (unsigned int[]){0x091C, 0x094D, 0x091E}, 3, 0},
    {"j~na", (unsigned int[]){0x091C, 0x094D, 0x091E}, 3, 0},
    {"shra", (unsigned int[]){0x0936, 0x094D, 0x0930}, 3, 0}, 

    // Dependent vowel matra
    {"i", (unsigned int[]){0x093F}, 1, 1},
    {"I", (unsigned int[]){0x0940}, 1, 1}, 
    {"ii", (unsigned int[]){0x0940}, 1, 1},
    {"ee", (unsigned int[]){0x0940}, 1, 1},
    {"u", (unsigned int[]){0x0941}, 1, 1},
    {"U", (unsigned int[]){0x0942}, 1, 1},
    {"uu", (unsigned int[]){0x0942}, 1, 1},
    {"RRi", (unsigned int[]){0x0943}, 1, 1},
    {"R^i", (unsigned int[]){0x0943}, 1, 1},
    {"LLi", (unsigned int[]){0x0962}, 1, 1},
    {"L^i", (unsigned int[]){0x0962}, 1, 1},
    {"e", (unsigned int[]){0x0947}, 1, 1},
    {"E", (unsigned int[]){0x0947}, 1, 1},
    {"ai", (unsigned int[]){0x0948}, 1, 1},
    {"o", (unsigned int[]){0x094B}, 1, 1},
    {"O", (unsigned int[]){0x094B}, 1, 1},
    {"au", (unsigned int[]){0x094C}, 1, 1},

    {"M", (unsigned int[]){0x0902}, 1, 0},
    {".m", (unsigned int[]){0x0902}, 1, 0},
    {".n", (unsigned int[]){0x0902}, 1, 0},
    {"H", (unsigned int[]){0x0903}, 1, 0},
    {".N", (unsigned int[]){0x0901}, 1, 0},
    {".h", (unsigned int[]){0x094D}, 1, 0},
    {".c", (unsigned int[]){0x0945}, 1, 0},
};

int is_consonant(unsigned int code)
{
    if ((code >= 0x0915 && code <= 0x0939) ||
        (code >= 0x0958 && code <= 0x095F))
        return 1;
    return 0;
}

int is_vowel_token(const char *token)
{
    return (strcmp(token, "a") == 0 ||
            strcmp(token, "A") == 0 ||
            strcmp(token, "i") == 0 ||
            strcmp(token, "I") == 0 ||
            strcmp(token, "ii") == 0 ||
            strcmp(token, "ee") == 0 ||
            strcmp(token, "u") == 0 ||
            strcmp(token, "U") == 0 ||
            strcmp(token, "uu") == 0 ||
            strcmp(token, "RRi") == 0 ||
            strcmp(token, "R^i") == 0 ||
            strcmp(token, "LLi") == 0 ||
            strcmp(token, "L^i") == 0 ||
            strcmp(token, "e") == 0 ||
            strcmp(token, "E") == 0 ||
            strcmp(token, "ai") == 0 ||
            strcmp(token, "o") == 0 ||
            strcmp(token, "O") == 0 ||
            strcmp(token, "au") == 0);
        }
        int is_dependent_vowel_codepoint(unsigned int code)
        {
            return (code == 0x093E || code == 0x093F || code == 0x0940 ||
                    code == 0x0941 || code == 0x0942 || code == 0x0943 ||
                    code == 0x0947 || code == 0x0948 || code == 0x094B || code == 0x094C);
        }
        

char *english_to_devnagari_str(const char *input) {
    char *buffer = malloc(1024);
    if (!buffer) return NULL;
    int pos = 0, out_pos = 0;
    int mappings_count = sizeof(mappings) / sizeof(mappings[0]);
    int len = strlen(input);
    int last_was_consonant = 0;
    while (pos < len) {
        int matched = 0;
        for (int l = 4; l >= 1; l--) {
            if (pos + l > len)
                continue;
            char substr[5];
            strncpy(substr, input + pos, l);
            substr[l] = '\0';
            for (size_t i = 0; i < mappings_count; i++) {
                if (strcmp(substr, mappings[i].eng) == 0) {
                    for (int j = 0; j < mappings[i].count; j++) {
                        unsigned int code = mappings[i].codes[j];
                        if (code <= 0x7F) {
                            buffer[out_pos++] = code;
                        } else if (code <= 0x7FF) {
                            buffer[out_pos++] = 0xC0 | (code >> 6);
                            buffer[out_pos++] = 0x80 | (code & 0x3F);
                        } else {
                            buffer[out_pos++] = 0xE0 | (code >> 12);
                            buffer[out_pos++] = 0x80 | ((code >> 6) & 0x3F);
                            buffer[out_pos++] = 0x80 | (code & 0x3F);
                        }
                    }
                    pos += l;
                    matched = 1;
                    last_was_consonant = is_consonant(mappings[i].codes[0]);
                    break;
                }
            }
            if (matched)
                break;
        }
        if (!matched) {
            buffer[out_pos++] = input[pos];
            last_was_consonant = 0;
            pos++;
        }
    }
    buffer[out_pos] = '\0';
    return buffer;
}

void output_utf8(FILE *out, unsigned int code) {
    if (code <= 0x7F)
        fputc(code, out);
    else if (code <= 0x7FF) {
        fputc(0xC0 | (code >> 6), out);
        fputc(0x80 | (code & 0x3F), out);
    } else {
        fputc(0xE0 | (code >> 12), out);
        fputc(0x80 | ((code >> 6) & 0x3F), out);
        fputc(0x80 | (code & 0x3F), out);
    }
}
void dfs_trie(tri_node *t, unsigned int *buffer, int depth, FILE *out);
void traverse_avl(node *n, unsigned int *buffer, int depth, FILE *out);

void traverse_avl(node *n, unsigned int *buffer, int depth, FILE *out) {
    if (!n) return;
    traverse_avl(n->left, buffer, depth, out);
    buffer[depth] = n->data;
    dfs_trie(n->tri_node, buffer, depth + 1, out);
    traverse_avl(n->right, buffer, depth, out);
}

void dfs_trie(tri_node *t, unsigned int *buffer, int depth, FILE *out) {
    if (t->isEnd) {
        for (int i = 0; i < depth; i++)
            output_utf8(out, buffer[i]);
        fprintf(out, "\n");
    }
    traverse_avl(t->avl_root, buffer, depth, out);
}

void search_prefix(tri_node *root, const char *prefix) {
    int len = strlen(prefix);
    unsigned int *codepoints = malloc((len + 1) * sizeof(unsigned int));
    int count = 0, pos = 0;
    while (prefix[pos]) {
        unsigned char c = prefix[pos];
        if (c <= 0x7F) { codepoints[count++] = c; pos++; }
        else if ((c & 0xE0) == 0xC0) {
            codepoints[count++] = ((c & 0x1F) << 6) | (prefix[pos+1] & 0x3F);
            pos += 2;
        }
        else if ((c & 0xF0) == 0xE0) {
            codepoints[count++] = ((c & 0x0F) << 12) |
                                    (((unsigned char)prefix[pos+1] & 0x3F) << 6) |
                                    (((unsigned char)prefix[pos+2] & 0x3F));
            pos += 3;
        }
        else { codepoints[count++] = 0xFFFD; pos++; }
    }
    
    tri_node *curr = root;
    int found = 1;
    for (int i = 0; i < count; i++) {
        node *n = curr->avl_root;
        node *found_node = NULL;
        while (n) {
            if (codepoints[i] < n->data)
                n = n->left;
            else if (codepoints[i] > n->data)
                n = n->right;
            else { found_node = n; break; }
        }
        if (!found_node) { found = 0; break; }
        curr = found_node->tri_node;
        if (!curr) { found = 0; break; }
    }
    if (!found)
        printf("No suggestions\n");
    else {
        printf("Suggestions:\n");
        unsigned int buffer[256];
        for (int i = 0; i < count; i++)
            buffer[i] = codepoints[i];
        dfs_trie(curr, buffer, count, stdout);
    }
    free(codepoints);
}

int main() {
    char *pre_str = read_file("preorder.txt");
    if (!pre_str) { fprintf(stderr, "Error reading preorder.txt\n"); return 1; }
    
    int pre_token_count;
    char **pre_tokens = tokenize(pre_str, &pre_token_count);
    free(pre_str);
    
    int index = 0;
    tri_node *trie_root = deserialize_trie_preorder(pre_tokens, &index);
    free_tokens(pre_tokens, pre_token_count);
    
    printf("Trie reconstructed .\n");
    
    char prefix_input[256];
    while (true) {
        printf("\nprefix: ");
        if (!fgets(prefix_input, sizeof(prefix_input), stdin))
            break;
        prefix_input[strcspn(prefix_input, "\n")] = '\0';
        
        char *devPrefix = english_to_devnagari_str(prefix_input);
        if (!devPrefix) {
            fprintf(stderr, "Conversion error\n");
            continue;
        }
        search_prefix(trie_root, devPrefix);
        free(devPrefix);
    }
    
    return 0;
}
