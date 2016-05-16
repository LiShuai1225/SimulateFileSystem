#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILENAME_LEN 20
#define COMMAND_LEN 10
#define INPUT_LEN 100
#define PATH_LEN 80
#define COMD_NUM 7
#define DIR_DEEPTH 20
typedef enum {DIR_0,FILE_1} FileType;
typedef enum {FALSE,TRUE} Bool;
char g_typename[][5] = {"DIR","FILE"};
char g_comdname[][COMMAND_LEN] = {"exit", "cd", "edit", "del", "dir", "md", "rd"};
enum {ROOT_0,BIN_1,USR_2,UNIX_3,ETC_4,LIB_5,USER_6,BIN_7,LIU_8,SUN_9,FTI_10};

struct FileNode{
    char filename[FILENAME_LEN];
    struct FileNode * parent, * child;
    struct FileNode * left, * right;
    FileType type;
};

char g_cnt_dir[PATH_LEN];
char * g_path;
char * g_cmd;
char * g_input;
char * g_param[5];
char * g_filename[10];
struct FileNode * g_root;
struct FileNode * g_cnt_node;

void PreOrderSearch(struct FileNode * root);
void TestInit();
void TestCheckPath();
void TestFindFile();

void Init();
void FreeMemory(struct FileNode * root);
struct FileNode * CreateNode(char *filename,FileType type);
void GetInput();
void CdComd();
struct FileNode * FindFile(struct FileNode * node, const char * filename);
Bool CheckPath(struct FileNode * root, struct FileNode ** desk);
void EditComd();
void InsertNode(struct FileNode *parent, struct FileNode *child);
void DirComd();
void DelComd();
void ExitComd();
void MdComd();
void DelDirComd();
void DelChild(struct FileNode * root);
void DelDirComdCase(struct FileNode * root);
void DelComdCase(struct FileNode * root);
void MdComdCase(struct FileNode * root, char **filename, size_t filename_len);
void EditComdCase(struct FileNode * root);
void OtherComd();

int main(){
    Init();
    void(*fun_table[])() = {ExitComd, CdComd, EditComd, DelComd, DirComd, MdComd, DelDirComd, OtherComd};
    for(;;){
        GetInput();
        int i;
        int flag = COMD_NUM;
        for(i = 0;i < COMD_NUM; i++)
            if(strcmp(g_cmd, g_comdname[i]) == 0){
                flag = i;
                fun_table[flag]();
                if(i == 0)
                    exit(0);
                break;
            }
        if(flag == COMD_NUM)
            fun_table[flag]();
    }
    exit(0);
}

void OtherComd(){
    printf("OtherComd : %s not found\n", g_cmd);
}

void DelDirComdCase(struct FileNode * root){
    struct FileNode * p;
    if(CheckPath(root, &p) == TRUE){
        if(p -> child){
            DelChild(p);
        }
        if(p -> left == NULL){
            p -> parent -> child = p -> right;
            if(p -> right)
                p -> right -> left = NULL;
            free(p);
        }
        else{
            p -> left -> right = p -> right;
            p -> right -> left = p -> left;
            free(p);
        }
    }
    else{
        printf("DelDirComd : %s not exit\n", g_path);
    }
}

void DelDirComd(){
    if(*g_path == '/'){
        DelDirComdCase(g_root);
    }
    else{
        DelDirComdCase(g_cnt_node);
    }
}

void DelChild(struct FileNode * root){
    if(root -> child == NULL)
        return;
    struct FileNode * node = root -> child;
    while(node){
        if(node -> type == DIR_0){
            DelChild(node);
            root = node;
            node = node -> right;
            free(root);
        }
        else{
            root = node;
            node = node -> right;
            free(root);
        }
    }
}

void MdComdCase(struct FileNode * root, char *filename[], size_t filename_len){
    struct FileNode * p;
    if(CheckPath(root, &p) == FALSE){
        size_t j;
        struct FileNode * node, * parent = root;
        for(j = 0; j < filename_len;j++){
            node = CreateNode(filename[j], DIR_0);
            InsertNode(parent, node);
            parent = node;
        }
    }
    else{
        printf("MdComd : dir already exits\n" );
    }
}

void MdComd(){
    char * filename[DIR_DEEPTH];
    char delim[] = "/";
    char * saveptr;
    char * tmp = (char *)malloc(sizeof(g_path));
    strcpy(tmp, g_path);
    char * ptr = strtok_r(tmp, delim, &saveptr);
    size_t i = 0;
    filename[i++] = ptr;
    while((ptr = strtok_r(NULL, delim, &saveptr)) != NULL)
        filename[i++] = ptr;
    if(*g_path == '/'){
        MdComdCase(g_root, filename, i);
    }
    else{
        MdComdCase(g_cnt_node, filename, i);
    }
    free(tmp);
}

void DelComdCase(struct FileNode * root){
    struct FileNode * p;
    if(CheckPath(root, &p) == FALSE || p -> type == DIR_0){
        printf("DelComd : g_path not exit\n" );
        return;
    }
    if(p -> left == NULL){
        p -> parent -> child = p -> right;
    }
    else{
        p -> left -> right = p -> right;
    }
    if(p -> right)
        p -> right -> left = p -> left;
}

void DelComd(){
    if(*g_path == '/'){
        DelComdCase(g_root);
    }
    else{
        DelComdCase(g_cnt_node);
    }
}

void DirComd(){
    struct FileNode * p = g_cnt_node -> child;
    while(p){
        printf("<%s>      %s\n", g_typename[p -> type], p -> filename);
        p = p -> right;
    }
}

void InsertNode(struct FileNode *parent, struct FileNode *child){
    child -> parent = parent;
    if(parent -> child){
        child -> right = parent -> child;
        parent -> child -> left = child;
    }
    parent -> child = child;
}

void EditComdCase(struct FileNode * root){
    char *last = strrchr(g_path, '/');
    struct FileNode * p;
    if(last == NULL){
        if(CheckPath(root, &p) == FALSE){
            if(root -> type == DIR_0){
                struct FileNode * node =CreateNode(g_path, FILE_1);
                InsertNode(root, node);
            }
            else{
                printf("EditCom : current dir is a file\n" );
            }
        }
        else{
            printf("EditComd : g_path already exits\n" );
        }
    }
    else{
        *last = '\0';
        if(CheckPath(root, &p) == TRUE){
            g_path = ++last;
            struct FileNode * tmp = p;
            if(CheckPath(tmp, &p) == FALSE){
                struct FileNode * node =CreateNode(g_path, FILE_1);
                InsertNode(tmp, node);
            }
            else{
                printf("EditComd : %s already exits\n",g_path );
            }
        }
        else{
            printf("EditComd : %s already exits\n",g_path );
        }
    }
}

void EditComd(){
    if(*g_path == '/'){
        EditComdCase(g_root);
    }
    else{
        EditComdCase(g_cnt_node);
    }
}

void CdComd(){
    struct FileNode * node;
    if(*g_path == '/'){
        if(strlen(g_path) == 1){
            g_cnt_node = g_root;
            strcpy(g_cnt_dir,g_path);
            return;
        }
        if(CheckPath(g_root, &node) == TRUE){
            g_cnt_node = node;
            strcpy(g_cnt_dir, g_path);
        }
        else{
            printf("CdComd : without this path\n");
        }
    }
    else{
        if(CheckPath(g_cnt_node, &node) == TRUE){
            if(g_cnt_node != g_root)
                strcat(g_cnt_dir, "/");
            strcat(g_cnt_dir, g_path);
            g_cnt_node = node;
        }
        else{
            printf("CdComd : without this path\n");
        }
    }
}

void TestCheckPath(){
    struct FileNode * p;
    printf("%d\n", CheckPath(g_root, &p));
}

void TestFindFile(){
    struct FileNode * p = FindFile(g_root, "lib");
    printf("%s\n", p -> filename);
}

Bool CheckPath(struct FileNode * root,struct FileNode ** desk){
    char * path = (char *)malloc(sizeof(g_path));
    strcpy(path, g_path);
    char delim[] = "/";
    char * saveptr;
    char * p = strtok_r(path, delim, &saveptr);
    size_t i = 0;
    g_filename[i++] = p;
    while((p = strtok_r(NULL, delim, &saveptr)))
        g_filename[i++] = p;
    size_t j;
    struct FileNode * child = root;
    for(j = 0; j < i; j++){
        if((child = FindFile(child, g_filename[j])) == NULL){
            *desk = NULL;
            return FALSE;
        }
    }
    *desk = child;
    return TRUE;
}



struct FileNode * FindFile(struct FileNode * node, const char * filename){
    if(node == NULL)
        return (struct FileNode *)0;
    struct FileNode * child = node -> child;
    while(child != NULL && strcmp(child -> filename, filename) != 0)//先判断再传参
        child = child -> right;
    return child;
}

void GetInput(){
    printf("%s %% ", g_cnt_dir);
    size_t len;
    getline(&g_input, &len, stdin);
    unsigned int i = 0;
    char delim[] = " \n";
    char * saveptr;
    char * p = strtok_r(g_input, delim, &saveptr);
    g_param[i++] = p;
    g_cmd = p;
    while((p = strtok_r(NULL, delim, &saveptr)))
        g_param[i++] = p;
    g_param[i] = NULL;
    if(i > 2)
        printf("%s\n", "cmd error");
    if(i == 2)
        g_path = g_param[1];
}

struct FileNode * CreateNode(char *filename,FileType type){
    struct FileNode * node = (struct FileNode *)malloc(sizeof(struct FileNode));
    strcpy(node -> filename, filename);
    node -> type = type;
    node -> parent = NULL;
    node -> child = NULL;
    node -> left = NULL;
    node -> right = NULL;
    return node;
}

void Init(){
    char name[][FILENAME_LEN] = {"/", "bin", "usr", "unix", "etc", "lib", "user", "bin", "liu", "sun", "fti"};
    size_t i;
    size_t len = sizeof(name)/sizeof(name[0]);
    struct FileNode * node_ptr [len];
    g_cnt_dir[0] = '/';
    g_cnt_dir[1] = '\0';
    for(i = 0; i < len; i++)
        node_ptr[i] = CreateNode(name[i], DIR_0);

    g_root = node_ptr[ROOT_0];
    node_ptr[UNIX_3] -> type = FILE_1;

    g_cnt_node = g_root;

    node_ptr[ROOT_0] -> child = node_ptr[BIN_1];

    node_ptr[BIN_1] -> parent = node_ptr[ROOT_0];
    node_ptr[BIN_1] -> right = node_ptr[USR_2];

    node_ptr[USR_2] -> child = node_ptr[LIB_5];
    node_ptr[USR_2] -> parent = node_ptr[ROOT_0];
    node_ptr[USR_2] -> left = node_ptr[BIN_1];
    node_ptr[USR_2] -> right = node_ptr[UNIX_3];

    node_ptr[UNIX_3] -> parent = node_ptr[ROOT_0];
    node_ptr[UNIX_3] -> left = node_ptr[USR_2];
    node_ptr[UNIX_3] -> right = node_ptr[ETC_4];

    node_ptr[ETC_4] -> parent = node_ptr[ROOT_0];
    node_ptr[ETC_4] -> left = node_ptr[UNIX_3];

    node_ptr[LIB_5] -> parent = node_ptr[USR_2];
    node_ptr[LIB_5] -> child = node_ptr[LIU_8];
    node_ptr[LIB_5] -> right = node_ptr[USER_6];

    node_ptr[USER_6] -> parent = node_ptr[USR_2];
    node_ptr[USER_6] -> left = node_ptr[LIB_5];
    node_ptr[USER_6] -> right = node_ptr[BIN_7];

    node_ptr[BIN_7] -> parent = node_ptr[USR_2];
    node_ptr[BIN_7] -> left = node_ptr[USER_6];

    node_ptr[LIU_8] -> parent = node_ptr[LIB_5];
    node_ptr[LIU_8] -> right = node_ptr[SUN_9];

    node_ptr[SUN_9] -> parent = node_ptr[LIB_5];
    node_ptr[SUN_9] -> left = node_ptr[LIU_8];
    node_ptr[SUN_9] -> right = node_ptr[FTI_10];

    node_ptr[FTI_10] -> parent = node_ptr[LIB_5];
    node_ptr[FTI_10] -> left = node_ptr[SUN_9];
}

void TestInit(){
    PreOrderSearch(g_root);
}

void PreOrderSearch(struct FileNode * root){
    printf("%s\n", root -> filename);
    if(root -> child)
        PreOrderSearch(root -> child);
    if(root -> right)
        PreOrderSearch(root -> right);
}


void FreeMemory(struct FileNode * root){
    if(root -> child)
        FreeMemory(root -> child);
    if(root -> right)
        FreeMemory(root -> right);
    free(root);
}

void ExitComd(){
    printf("thank you!\n" );
    FreeMemory(g_root);
}
