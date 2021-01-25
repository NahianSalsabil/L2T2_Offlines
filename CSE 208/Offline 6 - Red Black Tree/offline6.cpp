#include<bits/stdc++.h>
#define RED 'r'
#define BLACK 'b'
#define NIL NULL
using namespace std;

struct node{
    int data;
    char color;
    node *left, *right, *parent;
};

class RedBlackTree{
    ofstream outputfile;
    node* root;

public:
    node* getNewNode(int data);
    void leftRotate(node* x);
    void rightRotate(node* x);
    node* findPointer(int data);
    void insert(int data);
    void insertFixUp(node* z);
    bool Find(int data);
    void Delete(int data);
    void deletefixup(node* x);
    void printTree(node* root);
    void printTreeinFile(node* root);
    RedBlackTree();
    ~RedBlackTree();

};


node* RedBlackTree::getNewNode(int data){
    node *newNode = new node;
    node *leftnode = new node;
    node *rightnode = new node;
    newNode ->data = data;
    newNode ->left = leftnode;
    newNode ->right = rightnode;
    newNode ->parent = NULL;
    newNode ->color = RED;

    leftnode ->data = -1;
    leftnode ->parent = newNode;
    leftnode ->color = BLACK;

    rightnode ->data = -1;
    rightnode ->parent = newNode;
    rightnode ->color = BLACK;

    return newNode;

}

void RedBlackTree::leftRotate(node* x){
    node* y = x->right;
    x->right = y->left;
    y->left->parent = x;
    y->parent = x->parent;

    if(x ->parent == NULL)
        root = y;
    else if(x->parent->left == x)   /// if x left child
        x->parent->left = y;
    else if(x->parent->right == x)   /// if x right child
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(node* x){
    node* y = x->left;
    x->left = y->right;
    y->right->parent = x;
    y->parent = x->parent;

    if(x ->parent == NULL)
        root = y;
    else if(x->parent->left == x)   /// if x left child
        x->parent->left = y;
    else if(x->parent->right == x)   /// if x right child
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

node* RedBlackTree::findPointer(int data){
    node* x = root;
    node* y = NULL;

    while(x != NULL && x->data != -1){
        y = x;
        if(x->data > data)
            x = x->left;
        else if(x->data < data)
            x = x->right;
        else if(x->data == data)
            return x;
    }
    return y;

}

void RedBlackTree::insert(int data){
    node* z = getNewNode(data);
    node *y = findPointer(data);
    if(y == NULL)
        root = z;
    else if(y->data > data){
        y->left = z;
        z->parent = y;
    }
    else{
        y->right = z;
        z->parent = y;
    }
    insertFixUp(z);
    printTree(root);
    cout << endl;
    printTreeinFile(root);
    outputfile << endl;
}

void RedBlackTree::insertFixUp(node* z){
    node* y = NULL;
    if(z == root){
        z->color = BLACK;
        return;
    }
    while(z != root && z->parent->color == RED){
        if(z->parent == z->parent->parent->left){   ///   if z's parent is the left child
            y = z->parent->parent->right;   /// set y as z's uncle
            if(y->color == RED){      /// case 1 : z's uncle is red
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){   ///  case 2: z's uncle is black and z is right child
                    z = z->parent;
                    leftRotate(z);
                }
                    z->parent->color = BLACK;     /// case 3: z's uncle is black and z is left child
                    z->parent->parent->color= RED;
                    rightRotate(z->parent->parent);
            }
        }
        else if(z->parent == z->parent->parent->right){   /// z's parent is right child
            y = z->parent->parent->left;   /// set y as z's uncle
            if(y->color == RED){      /// case 1 : z's uncle is red
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){   ///  case 2: z's uncle is black and z is left child
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;     /// case 3: z's uncle is black and z is right child
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

bool RedBlackTree::Find(int data){
    node* x = root;
    while(x != NULL && x->data != -1){
        if(x->data > data)
            x = x->left;
        else if(x->data < data)
            x = x->right;
        else if(x->data == data){
            outputfile << "True\n";
            return true;
        }
    }
    outputfile << "False\n";
    return false;

}

void RedBlackTree::Delete(int data){
    node* x = NULL;
    node* y = NULL;
    node* z = findPointer(data);
    if(z->left->data == -1 || z->right->data == -1)
        y = z;
    else{
        y = z->right;
        while(y->left->data != -1){
            y = y->left;
        }
    }
    if(y->left->data != -1)
        x = y->left;
    else
        x = y->right;
    x->parent = y->parent;
    if(y == root)
        root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else if(y == y->parent->right)
        y->parent->right = x;
    if(y != z)
        z->data = y->data;
    if(y->color == BLACK)
        deletefixup(x);

    printTree(root);
    cout << endl;
    printTreeinFile(root);
    outputfile << endl;
}

void RedBlackTree::deletefixup(node* x){
    node* w = NULL;
    while(x->color == BLACK && x != root){
        if(x == x->parent->left){    /// x is left child
            w = x->parent->right;
            if(w->color == RED){      /// w is red
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){    /// w's children are both black
                w->color = RED;
                x = x->parent;
            }
            else{
                if(w->right->color == BLACK){   /// case 3
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;   /// case 4
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }

        }
        else if(x == x->parent->right){
            w = x->parent->left;
            if(w->color == RED){      /// w is red
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){    /// w's children are both black
                w->color = RED;
                x = x->parent;
            }
            else{
                if(w->left->color == BLACK){   /// case 3
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;   /// case 4
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}


void RedBlackTree::printTree(node* root){
    node *x = root;
    int flag = 0;
    if(x->data == -1)
        return;

    if(x->left->data == -1 && x->right->data == -1){
        cout << x->data << ":" << x->color;
        return;
    }
    cout << x->data << ":" << x->color << "(";
    printTree(x->left);
    cout << ")(";
    printTree(x->right);
    cout << ")";
}

void RedBlackTree::printTreeinFile(node* root){
    node *x = root;
    if(x->data == -1)
        return;
    if(x->left->data == -1 && x->right->data == -1){
        outputfile << x->data << ":" << x->color;
        return;
    }
    outputfile << x->data << ":" << x->color << "(";
    printTreeinFile(x->left);
    outputfile << ")(";
    printTreeinFile(x->right);
    outputfile << ")";
}


RedBlackTree::RedBlackTree(){
    root = NULL;
    outputfile.open("output.txt");
}

RedBlackTree::~RedBlackTree(){
    delete root;
}


int main(){
    RedBlackTree rbt;
    ifstream inputFile;
    inputFile.open("input.txt");
    char op;
    int element;
    while(inputFile >> op >> element){
        if(op == 'I'){ /// Insert
            rbt.insert(element);
        }
        else if(op == 'F'){ /// search
            bool found = rbt.Find(element);
            if(found) cout << "True\n";
            else cout << "False\n";
        }
        else if(op == 'D'){ ///delete
           rbt.Delete(element);
        }
        else
            break;
    }
    return 0;
}
