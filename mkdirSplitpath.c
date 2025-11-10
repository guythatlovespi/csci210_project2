#include "types.h" 

extern struct NODE* root;
extern struct NODE* cwd;

// find a child(frisk) node by name within a parent directory
static struct NODE* findChild(struct NODE* parent, char* name) {
    // Ensure parent is a valid directory
    if (parent == NULL || parent->fileType != 'D') return NULL;

    // Traverse the sibling list
    struct NODE* child = parent->childPtr;
    while (child) {
        // Found frisk
        if (strcmp(child->name, name) == 0) return child;
        child = child->siblingPtr;
    }

    //Failed to find a child
    // printf("No child\n");
    return NULL; 
}


//make directory
void mkdir(char pathName[]) {

    // Do we got a pathy path path??
    if (strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    // Base and Directory names
    char baseName[64];
    char dirName[128];

    struct NODE* parent = splitPath(pathName, baseName, dirName);

    // If invalid path then return
    if (parent == NULL) return;

    // If directory already exists, return
    if (findChild(parent, baseName) != NULL) {
        printf("MKDIR ERROR: directory %s already exists\n", pathName);
        return;
    }

    // If passes edge cases, start making nodes
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    if (newNode == NULL) return;
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->parentPtr = parent;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;

    // Link the new node into the parent's child list as the last siblng
    if (parent->childPtr == NULL) {
        // first child
        parent->childPtr = newNode;
    } else {
        // Loop to end
        struct NODE* currentChild = parent->childPtr;
        while (currentChild->siblingPtr != NULL) {
            currentChild = currentChild->siblingPtr;
        }

        // Add new node to the end
        currentChild->siblingPtr = newNode;
    }

    // A new node has been born
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}


//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    
    //Store temperary path
    char tempPath[128];

    // Use a temporary copy for parsing
    strcpy(tempPath, pathName); 

    // Find the last '/'
    char* lastSlash = strrchr(tempPath, '/');

    //Get directory name and base name from path

    //Is root?
    if (strcmp(tempPath, "/") == 0) {
        // pathName is "/"
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    // Did they provide no slash?
    } else if (lastSlash == NULL) {
        strcpy(dirName, "");
        strcpy(baseName, tempPath);

        // Then current directory
        return cwd;
    // Is the parent root?
    } else if (lastSlash == tempPath) {
        strcpy(dirName, "/");
        strcpy(baseName, lastSlash + 1); 

        return root;
    } else {
        
        strcpy(baseName, lastSlash + 1);
        *lastSlash = '\0'; 
        strcpy(dirName, tempPath); 
    }

    // Make sure to find parent node through directory
    struct NODE* current;
    
    // Are we in root (absolute) or current directory?
    if (dirName[0] == '/') {
        current = root;
    } else {
        current = cwd;
    }

    // pointer to save where we're are
    //char *saveptr; 
    
    // TODO: strtok keeps failing, fix
    char* token = strtok(dirName, "/"); 

    while (token != NULL) {
        struct NODE* child = findChild(current, token);

        // Path validation
        if (child == NULL || child->fileType != 'D') {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL;
        }
        
        // Move down the tree
        current = child;

        // pass our save pointer again.
        token = strtok(NULL, "/"); 
    }

    // current shoudl point to the node represented by the directory
    return current;
}