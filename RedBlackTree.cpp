// RedBlackTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Progream creates a Red Black Tree
// Author: Pranav Sharma
// Date: 1/21/2021
// 

#include <iostream>
#include <string>
using namespace std;
// this struct defines the properties of the node
struct Node {
public:
    bool isRed;
    int data;
    Node* parent;
    Node* left;
    Node* right;
};

// general functions used in the program
bool isParent(Node* n);
bool checkParent(Node* n);
void readFile(char file[], int numbers, Node* &root);
void printTree(Node* head, int spacing);
Node* search(Node* head, int num);

// specific node functions, ideas from wikipedia
Node* getParent(Node* n);
Node* getGrandParent(Node* n);
Node* getSibling(Node* n);
Node* getUncle(Node* n);
void rotateLeft(Node* n);
void rotateRight(Node* n);

// inserting functions, ideas from wikipedia
Node* insertNode(Node* root, Node* node);
void insertRecurse(Node* root, Node* node);
void repairTree(Node* node);

// specific cases for inserting, from wikipedia
void insertCase1(Node* &n);
void insertCase2(Node* &n);
void insertCase3(Node* &n);
void insertCase4(Node* &n);

// deletion functions, ideas from geeksforgeeks.com
void replaceNode(Node* n, Node* child);
Node* minValueNode(Node* root);
void swapvalues(Node* u, Node* v);
bool isonLeft(Node* x);
void fixdoubleblack(Node* x, Node* root);
Node* succesor(Node* n);
Node* BSTreplace(Node* n);
void deleteN(Node* v, Node* root);

int main()
{
    // introduces user to progream and generates vairables
    char* input = new char();
    bool playing = true;
    cout << "Welcome to my Red-Black Tree Program!\n";
    Node* root = new Node();

    // loops through asking user what they would like to do until they quit
    while (playing) {
        cout << "What would you like to do? (ADD, PRINT, READ, DELETE, SEARCH or QUIT)\n";
        cin.getline(input, 80, '\n');
        // if user wants to add...
        if (strcmp(input, "ADD") == 0) {
            // program asks for number then inserts it
            int num;
            cout << "What number would you like to add?\n";
            cin >> num;
            cin.get();
            // assigns first number to root
            if (root->data == 0) {
                root->data = num;
            }
            else {
                Node* n = new Node();
                n->data = num;
                root = insertNode(root, n);
            }
        }
        // prints the Red Black Tree
        else if (strcmp(input, "PRINT") == 0) {
            printTree(root, 0);
        }
        // reads from a file and adds a certain number of numbers to the tree
        else if (strcmp(input, "READ") == 0) {
            char file[20];
            int num;
            cout << "How many numbers would you like to read from a file?\n";
            cin >> num;
            cin.get();
            cout << "Enter the file you would like to open, in this program there is a file called 'Number.txt'\n";
            cin.getline(file, 80, '\n');
            readFile(file, num, root);
        }
        // if user wants to delete a node
        else if (strcmp(input, "DELETE") == 0) {
            int num;
            cout << "Which number would you like to delete?\n ";
            cin >> num;
            cin.get();
            Node* n = search(root, num);
            if (n == NULL) {
                cout << "It doesn't look like this number is part of the tree\n";
            }
            else {
                deleteN(n, root);
            }
            // check which deletion case needs to be run
            /*if (checkParent(n) == true) {
                deleteChild(root, n);
            }
            else {
                root = deleteNode(root, num);
            }*/
        }
        // searches for a certain node
        else if (strcmp(input, "SEARCH") == 0) {
            int num;
            cout << "Which number would you like to search for?\n ";
            cin >> num;
            cin.get();
            // results are printed using search function
            if (search(root, num) != NULL ) {
                cout << "This number is in the binary tree!\n";
            }
            else {
                cout << "It looks like this number is not in the binary tree.\n";
            }
        }
        // quits the program
        else if (strcmp(input, "QUIT") == 0) {
            cout << "Alrighty! Adios\n";
            playing = false;
        }
        // user didn't enter a proper command :(
        else {
            cout << "Looks like you didn't enter a proper command :(\n";
        }
    }
}
// checks if the node enteres is a non-leaf node
bool isParent(Node* n){
    if (n != NULL) {
        if (n->left != NULL) {
            return true;
        }
        if (n->right != NULL) {
            return true;
        }
    }
    return false;
}
// checks if speciial deletion case needs to be run
bool checkParent(Node* n) {
    if (isParent(n->right) == true && isParent(n->left) == true) {
        return false;
    }
    else {
        return true;
    }
}
// reads in a file and adds the number indicated by the user to the RBTree
void readFile(char file[], int numbers, Node* &root) {
    FILE* pfile = NULL;
    char line[1000];
    // opens the file with the given name from user
    pfile = fopen(file, "r");
    // alerts if file is unreadable
    if (pfile == NULL)
    {
        cout << "Error in opening the file Numbers.txt";
        return;
    }

    // goes through each line, and stores whats beween the commas in the integer array
    int index = 0;
    bool reading = true;
    while (fgets(line, 1000, pfile) && reading == true) {
        char* buffer = new char[20];
        memset(buffer, '\0', 20);
        int pos = 0;
        while (buffer != NULL && reading == true) {
            buffer = strchr(line + pos, ',');
            if (buffer != NULL) {
                char lineBuffer[20];
                memset(lineBuffer, '\0', 20);
                strncpy(lineBuffer, line + pos, sizeof(line - buffer));
                if (root->data == 0) {
                    root->data = stoi(lineBuffer);
                }
                else {
                    Node* n = new Node();
                    n->data = stoi(lineBuffer);
                    root = insertNode(root, n);
                }
                index++;
                if (index == numbers) {// stops when intended numbers are read
                    reading = false;
                    return;
                }
                pos = (buffer - line) + 1;
            }
        }
    }
}
// prints the binary tree (sideways) using spaces
void printTree(Node* head, int spacing) {
    if (head != NULL) {
        spacing = spacing + 10;
        printTree(head->right, spacing);
        cout << endl;
        for (int i = 10; i < spacing; i++) {
            cout << " ";
        }
        if(head->isRed == true){
            cout << "\033[1m\033[31m" << head->data << endl << "\033[0m";
        }
        else{
            cout << "\033[1m\033[37m" << head->data << endl << "\033[0m";
        }
        printTree(head->left, spacing);
    }
}
// searches for a node based on a number, returns the node if it exists
Node* search(Node* head, int num) {
    Node* current = head;
    // follows the pathway of the tree, if the number is bigger than the root it goes right and left if it is smaller
    if (current != NULL) {
        if (current->data == num) {
            return current;
        }
        else if (current->data < num) {
           current = search(current->right, num);
        }
        else {
          current = search(current->left, num);
        }
    }
    if (current == head) {
        return NULL;
    }
    else {
        return current;
    }
}
// gets the parent of any given node
Node* getParent(Node* n) {
    if (n == NULL) {
        return NULL;
    }
    else {
        return n->parent;
    }
}
// gets the grand parent of any given node
Node* getGrandParent(Node* n) {
    return getParent(getParent(n));
}
// returns the sibling of a node
Node* getSibling(Node* n) {
    Node* parent = getParent(n);

    if (parent == NULL) {
        return NULL;
    }

    if (n == parent->right) {
        return parent->left;
    }
    else {
        return parent->right;
    }
}
// returns the uncle of a node
Node* getUncle(Node* n) {
    Node* parent = getParent(n);
    return getSibling(parent);
}
// rotates the tree left when the condition appears
void rotateLeft(Node* n) {
    Node* newNode = n->right;
    Node* parent = getParent(n);
    // makes sure newNode is not null before shifting
    if (newNode != NULL) { 
        n->right = newNode->left;
        newNode->left = n;
        n->parent = newNode;
        if (n->right != NULL) {
            n->right->parent = n;
        }

        if (parent != NULL) {
            if (n == parent->left) {
                parent->left = newNode;
            }
            else if (n == parent->right) {
                parent->right = newNode;
            }
        }
        newNode->parent = parent;
    }
}
// rotates the tree right
void rotateRight(Node* n) {
    Node* newNode = n->left;
    Node* parent = getParent(n);
    if (newNode != NULL) {
        n->left = newNode->right;
        newNode->right = n;
        n->parent = newNode;
        if (n->left != NULL) {
            n->left->parent = n;
        }

        if (parent != NULL) {
            if (n == parent->left) {
                parent->left = newNode;
            }
            else if (n == parent->right) {
                parent->right = newNode;
            }
        }
        newNode->parent = parent;
    }
}
// inserts a node into the tree
Node* insertNode(Node* root, Node* node) {
    insertRecurse(root, node);
    repairTree(node);

    root = node;
    while (getParent(root) != NULL) {
        root = getParent(root);
    }
    return root;
}
// recursively decends the tree until a leaf is found
void insertRecurse(Node* root, Node* node) {
    if (root != NULL) {
        if (node->data < root->data) {
            if (root->left != NULL) {
                insertRecurse(root->left, node);
                return;
            }
            else {
                root->left = node;
            }
        }
        else {
            if (root->right != NULL) {
                insertRecurse(root->right, node);
                return;
            }
            else {
                root->right = node;
            }
        }
    }
    // intializes conditions for a new node
    node->parent = root;
    node->left = NULL;
    node->right = NULL;
    node->isRed = true;
}

// makes sure tree fits all conditions after each move
void repairTree(Node* n) {
    if (getParent(n) == NULL) {
        insertCase1(n);
    }
    else if (getParent(n)->isRed == false) {
        insertCase2(n);
    }
    else if (getUncle(n) != NULL && getUncle(n)->isRed == true) {
        insertCase3(n);
    }
    else {
        insertCase4(n);
    }
}
// The current node is at the root of tree and must be repainted to black
void insertCase1(Node* &n) {
    n->isRed = false;
}
// tree remains valid and no action is needed
void insertCase2(Node* &n) {
    return;
}
// Parent and uncle are both red and must be repainted red. Granparents becomes red too.
void insertCase3(Node*& n) {
    getParent(n)->isRed = false;
    getUncle(n)->isRed = false;
    getGrandParent(n)->isRed = true;
    repairTree(getGrandParent(n));
}
// Rotates the new node N to the grandparent position. Left rotation switches node with parent and the right rotation switches parents and gparent
void insertCase4(Node*& n) {
    Node* p = getParent(n);
    Node* g = getGrandParent(n);

    if (n == p->right && p == g->left) {
        rotateLeft(p);
        n = n->left;
    }
    else if (n == p->left && p == g->right) {
        rotateRight(p);
        n = n->right;
    }

    p = getParent(n);
    g = getGrandParent(n);

    if (n == p->left) {
        rotateRight(g);
    }
    else {
        rotateLeft(g);
    }

    p->isRed = false;
    g->isRed = true;
}
// replaces a node by connection relationship between granparent and child
void replaceNode(Node* n, Node* child) {
    child->parent = n->parent;
    if (n == n->parent->left) {
        n->parent->left = child;
    }
    else {
        n->parent->right = child;
    }
}



// finds the smallest node in the Tree
Node* minValueNode(Node* root) {
    Node* current = root;
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    return current;
}

//new deletion try:

// swaps the values of two nodes
void swapvalues(Node* u, Node* v) {
    int temp = u->data;
    u->data = v->data;
    v->data = temp;
}
// returns true if node is a left child
bool isonLeft(Node* x) {
    Node* parent = getParent(x);

    if (x == parent->left) {
        return true;
    }
    else {
        return false;
    }
}
// fixes tree if there are two black nodes 
void fixdoubleblack(Node* x, Node* root) {
    if (x == root) {
        // at root
        return;
    }

    Node* sib = getSibling(x);
    Node* parent = getParent(x);

    if (sib == NULL) {
        // if theres no sibling double black gets pushed
        fixdoubleblack(parent, root);
    }
    else {
        if (sib->isRed == true) {
            // if sib red
            parent->isRed = true;
            sib->isRed = false;
            if (isonLeft(sib) == true) {
                // left case
                rotateRight(parent);
            }
            else {
                // right case
                rotateLeft(parent);
            }
            fixdoubleblack(x, root);
        }
        else {
            // if sib black
            if ((sib->right != NULL && sib->right->isRed == true) || (sib->left != NULL && sib->left->isRed == true)) {// might put null ptr exception
                // 1 red child
                if (sib->left != NULL && sib->left->isRed == true) {
                    if (isonLeft(sib)) {
                        // left left
                        sib->left->isRed = sib->isRed;
                        sib->isRed = parent->isRed;
                        rotateRight(parent);
                    }
                    else {
                        // right left
                        sib->left->isRed = parent->isRed;
                        rotateRight(sib);
                        rotateLeft(parent);
                    }
                }
                else {
                    // left right
                    if (isonLeft(sib) == true) {
                        sib->right->isRed = parent->isRed;
                        rotateLeft(sib);
                        rotateRight(parent);
                    }
                    else {
                        // right left
                        sib->right->isRed = sib->isRed;
                        sib->isRed = parent->isRed;
                        rotateLeft(parent);
                    }
                }
                parent->isRed = false;
            }
            else {
                // 2 black children
                sib->isRed = true;
                if (parent->isRed == false) {
                    fixdoubleblack(parent, root);
                }
                else {
                    parent->isRed = false;
                }
            }
        }
    }
}
// find node that do not have a left child 
 // in the subtree of the given node 
Node* succesor(Node* n) {
    Node* temp = n;
    while (temp->left != NULL) {
        temp = temp->left;
    }
    return temp;
}
// finds the node that replcaes the deleted node 
Node* BSTreplace(Node* n) {
    //when node has 2 children
    if (n->right != NULL && n->left != NULL) {
        return succesor(n->right);
    }
    // if leaf
    if (n->left == NULL && n->right == NULL) {
        return NULL;
    }
    // if single child
    if (n->left != NULL) {
        return n->left;
    }
    else {
        return n->right;
    }
}
// function deletes the node
void deleteN(Node* v, Node* root) {
    Node* u = BSTreplace(v);

    bool UVBlack;
    if (((u == NULL || u->isRed == false) && v->isRed == false)) {
        UVBlack = true;
    }
    else {
        UVBlack = false;
    }
    Node* parent = v->parent;

    if (u == NULL) {
        if (v == root) {
            root == NULL;
        }
        else {
            if (UVBlack == true) {
                fixdoubleblack(v, root);
            }
            else {
                Node* sib = getSibling(v);
                if (sib != NULL) {
                    sib->isRed = true;
                }
            }

            //delete v from tree
            if (isonLeft(v) == true) {
                parent->left = NULL;
            }
            else {
                parent->right = NULL;
            }
        }
        delete v;
        return;
    }

    // if v has 1 child
    if (v->left == NULL || v->right == NULL) {
        if (v == root) {
            v->data = u->data;
            v->left = NULL;
            v->right = NULL;
            delete u;
        }
        else {
            if (isonLeft(v) == true) {
                parent->left = u;
            }
            else {
                parent->right = u;
            }
            delete v;
            u->parent = parent;
            if (UVBlack == true) {
                fixdoubleblack(u, root);
            }
            else {
                u->isRed = false;
            }
        }
        return;
    }
    swapvalues(u, v);
    deleteN(u, root);
}