#include<bits/stdc++.h>
#define NIL NULL
using namespace std;

struct node{
    int data, degree;
    node *child, *sibling, *parent;
};

class BinomialHeap{
    node* heap;

public:
    node* getNewNode(int data);
    void insert(node* h);
    int FindMin();
    node* reverseHeap(node* temp, node* h);
    int ExtractMin();
    void LinkTree(node* y, node* z);
    node* MergeHeap(node* h1, node* h2);
    void Union();
    node* UnionHeap(node* h1, node* h2);
    void printHeap();
    BinomialHeap();
    ~BinomialHeap();

};

node* BinomialHeap::getNewNode(int data){
    node *newNode = new node;
    newNode ->data = data;
    newNode -> degree = 0;
    newNode ->parent = NULL;
    newNode ->child = NULL;
    newNode ->sibling = NULL;

    return newNode;

}

void BinomialHeap::insert(node* h){
    node *temp = h;
    heap = UnionHeap(heap,temp);
}

void BinomialHeap::Union(){
    vector<int> keys;
    string input;
    getline(cin, input);
    istringstream is(input);
    int num;
    while(is>>num)
        keys.push_back(num);
    node* new_heap = NULL;

    for(int i = 0; i < keys.size(); i++){
        node* n = getNewNode(keys.at(i));
        new_heap = UnionHeap(new_heap,n);
    }
    heap = UnionHeap(heap,new_heap);
}

node* BinomialHeap::UnionHeap(node* h, node* temp){
    node* temp_heap;
    temp_heap = MergeHeap(h,temp);

    if(temp_heap == NULL)
        return temp_heap;

    node* prev_x = NULL;
    node* x = temp_heap;
    node* next_x = x ->sibling;

    while(next_x != NULL){
        if(x ->degree != next_x ->degree || (next_x ->sibling != NULL && next_x -> sibling ->degree == x ->degree)){
           prev_x = x;
           x = next_x;
        }
        else{
            if(x ->data <= next_x ->data){
                x ->sibling = next_x ->sibling;
                LinkTree(next_x,x);
            }
            else{
                if(prev_x == NULL){
                    temp_heap = next_x;
                }
                else
                    prev_x ->sibling = next_x;
                LinkTree(x,next_x);
                x = next_x;
            }
        }
        next_x = x ->sibling;
    }
    return temp_heap;
}

node* BinomialHeap::MergeHeap(node* heap, node* temp){
    node* new_heap;
    node* y = heap;
    node* z = temp;
    node* a, *b;

    if(y != NULL){
        if(z != NULL){
            if(y->degree <= z->degree)
                new_heap = y;
            else
                new_heap = z;
        }
        else
            new_heap = y;
    }
    else
        new_heap = z;

    while(y != NULL && z!= NULL){
        if(y->degree < z->degree)
            y = y->sibling;
        else if(y->degree == z->degree){
            a = y ->sibling;
            y->sibling = z;
            y = a;
        }
        else{
            b = z->sibling;
            z->sibling = y;
            z = b;
        }
    }
    return new_heap;
}

void BinomialHeap::LinkTree(node* y, node* z){
    y ->parent = z;
    y ->sibling = z ->child;
    z ->child = y;
    z ->degree++;
}

int BinomialHeap::FindMin(){
    node* y = NULL;
    node* x = heap;
    int min = 999999;
    while(x != NULL){
        if(x->data < min){
            min = x->data;
            y = x;
        }
        x = x->sibling;
    }
    if(y == NULL)
        return -1;
    return y->data;
}

node* BinomialHeap::reverseHeap(node* temp,node* cnode){
    if(cnode->sibling != NULL){
        temp = reverseHeap(temp, cnode->sibling);
        temp->sibling = cnode;
    }
    else
        return cnode;
}

int BinomialHeap::ExtractMin(){
    node* y = heap;
    node* z = NULL;
    node* t = y;
    node* temp = NULL;
    node* h = NULL;
    if(y == NULL)
        return -1;
    int min = y->data;
    while(t->sibling != NULL){   /// find min
        if(t->sibling->data < min){
            z = t;
            y = t->sibling;
            min = t->sibling->data;
        }
        t = t->sibling;
    }
    if(z == NULL && y->sibling != NULL)
        h = y->sibling;
    else if(z != NULL){
        h = heap;
        z->sibling = y->sibling;
    }
    if(y->child != NULL){
        temp = reverseHeap(temp,y->child);
        y->child->sibling = NULL;
    }
    heap = UnionHeap(h,temp);
    return min;
}

void BinomialHeap::printHeap(){
    queue<node*> hqueue;
    node* y = heap;
    node* temp = NULL;
    cout << "Printing Binomial Heap...\n";
    while(y != NULL){
        cout << "Binomial Tree, B" << y->degree << endl;
        hqueue.push(y);
        for(int i = 0; i <= y->degree; i++){
            cout << "Level " << i << " :";
            int qsize = hqueue.size();
            for(int i = 0; i < qsize; i++){
                cout << " " << hqueue.front()->data;
                temp = hqueue.front() ->child;
                hqueue.pop();
                while(temp!= NULL){
                    hqueue.push(temp);
                    temp = temp ->sibling;
                }
            }
            cout << endl;
        }
        y = y->sibling;
    }
}


BinomialHeap::BinomialHeap(){
    heap = NULL;
}

BinomialHeap::~BinomialHeap(){
    delete heap;
}



int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    char op;
    int element;
    BinomialHeap bh;
    while(cin >> op){
        if(op == 'I'){ /// Insert
            cin >> element;
            node* new_node = bh.getNewNode(element);
            bh.insert(new_node);
        }
        else if(op == 'U'){ /// union
            bh.Union();
        }
        else if(op == 'F'){ /// find min
            int find_min = bh.FindMin();
            if(find_min > 0) cout << "Find-Min returned " << find_min << endl;
        }
        else if(op == 'E'){ ///Extract min
            int find_min = bh.ExtractMin();
            if(find_min > 0) cout << "Extract-Min returned " << find_min << endl;
        }
        else if(op == 'P'){
            bh.printHeap();
        }
        else
            break;
    }
    return 0;
}
