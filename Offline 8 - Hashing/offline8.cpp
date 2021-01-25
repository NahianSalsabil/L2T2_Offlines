#include<bits/stdc++.h>
using namespace std;

struct keyvaluepair{
    int value;
    string key;
    keyvaluepair *prevNeighbour, *afterNeighbour;
};

class hashtable{
    int value,length,hashfunction,collision = 0,C1 = 7, C2 = 13;
    float probe = 0;
    keyvaluepair **table;
    string *words;

public:
    hashtable(int length,int hashfunction);
    ~hashtable();
    unsigned int hashfunction1(string key);
    unsigned int hashfunction2(string key);
    unsigned int hashfunctionAux(string key);
    int totalCollision();
    int totalProbe();
    void insert(string key,int method);
    void insertChain(string key,int value);
    int searchChain(string key);
    int deleteChain(string key);
    void insertDouble(string key,int value);
    int searchDouble(string key);
    int deleteDouble(string key);
    void insertCustom(string key,int value);
    int searchCustom(string key);
    int deleteCustom(string key);
};

hashtable::hashtable(int length,int hashfunction){
    table = new keyvaluepair*[length];
    for(int i = 0; i < length; i++)
        table[i] = NULL;
    value = 1;
    this->hashfunction = hashfunction;
    this->length = length;
    words = new string[length];
    for(int i = 0; i < length; i++)
        words[i] = "";
}

unsigned int hashtable::hashfunction1(string key){
    unsigned int h = 2166136261;
    for(int i = 0; i < key.length(); i++)
        h = (h * 16777619) ^ key[i];
    return h % length;
}

unsigned int hashtable::hashfunction2(string key){
    unsigned int h = 0;
    for(int i = 0; i < key.length(); i++)
    {
        h += key[i];
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h % length;
}

unsigned int hashtable::hashfunctionAux(string key){
    unsigned int h = 0;
    for(int i = 0; i < key.length(); i++)
        h ^= (h << 5) + (h >> 2) + key[i];

    return h;
}

int hashtable::totalCollision(){
    return collision;
}

int hashtable::totalProbe(){
    return probe;
}

void hashtable::insert(string key,int method){
    if(method == 1)
        insertChain(key,value);
    else if(method == 2)
        insertDouble(key,value);
    else
        insertCustom(key,value);
    value++;
}

void hashtable::insertChain(string key,int value){
    int index;
    if(hashfunction == 1)
        index = hashfunction1(key);
    else
        index = hashfunction2(key);
    keyvaluepair *kvpair = new keyvaluepair;
    kvpair->key = key;
    kvpair->value = value;
    kvpair->prevNeighbour = NULL;
    kvpair->afterNeighbour = NULL;
    if(table[index] == NULL){
        table[index] = kvpair;
    }
    else{
        keyvaluepair *temp1 = new keyvaluepair;
        keyvaluepair *temp2 = new keyvaluepair;
        temp2 = table[index];
        temp1 = table[index]->afterNeighbour;
        while(temp1!= NULL){
            temp2 = temp1;
            temp1 = temp2->afterNeighbour;
        }
        temp2->afterNeighbour = kvpair;
        kvpair->prevNeighbour = table[index];
        collision++;
    }
}

int hashtable::searchChain(string key){
    int index;
    if(hashfunction == 1)
        index = hashfunction1(key);
    else
        index = hashfunction2(key);
    keyvaluepair *temp = NULL;
    if(table[index] == NULL){
        probe++;
        //cout << "key not in table\n";
        return 0;
    }
    if(table[index]->key == key){
        probe++;
        return 1;
    }

    if(table[index]->afterNeighbour != NULL){
        temp = table[index]->afterNeighbour;
        while(temp!= NULL){
            probe++;
            if(temp->key == key)
                return 1;
            temp = temp->afterNeighbour;
        }
    }
    return 0;
}

int hashtable::deleteChain(string key){
    int index;
    if(hashfunction == 1)
        index = hashfunction1(key);
    else
        index = hashfunction2(key);
    keyvaluepair *temp = NULL;
    if(table[index] == NULL){
        //cout << "key not in table\n";
        return 0;
    }
    if(table[index]->key == key){
        if(table[index]->afterNeighbour!= NULL){
            table[index] = table[index]->afterNeighbour;
            table[index]->prevNeighbour = NULL;
        }
        else{
            table[index] = NULL;
        }
        return 1;
    }
    if(table[index]->afterNeighbour != NULL){
        temp = table[index]->afterNeighbour;
        while(temp != NULL){
            if(temp->key == key){
                temp->prevNeighbour->afterNeighbour = temp->afterNeighbour;
                if(temp->afterNeighbour != NULL)
                    temp->afterNeighbour->prevNeighbour = temp->prevNeighbour;
                return 1;
            }
            temp = temp->afterNeighbour;
        }
    }
    return 0;
}

void hashtable::insertDouble(string key,int value){
    keyvaluepair *kvpair = new keyvaluepair;
    kvpair->key = key;
    kvpair->value = value;
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + i * hashfunctionAux(key)) % length;
        else
            index = (hashfunction2(key) + i * hashfunctionAux(key)) % length;
        if(table[index] == NULL){
            table[index] = kvpair;
            return;
        }
        collision++;
    }
    //cout << "hash table overflow\n";
}

int hashtable::searchDouble(string key){
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + i * hashfunctionAux(key)) % length;
        else
            index = (hashfunction2(key) + i * hashfunctionAux(key)) % length;
        if(table[index]->key == key){
            return index;
        }
        probe++;
    }
    return -1;
}

int hashtable::deleteDouble(string key){
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + i * hashfunctionAux(key)) % length;
        else
            index = (hashfunction2(key) + i * hashfunctionAux(key)) % length;
        if(table[index]->key == key){
            table[index] = NULL;
            return index;
        }
    }
    return -1;

}

void hashtable::insertCustom(string key,int value){
    keyvaluepair *kvpair = new keyvaluepair;
    kvpair->key = key;
    kvpair->value = value;
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        else
            index = (hashfunction2(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        if(table[index] == NULL){
            table[index] = kvpair;
            return;
        }
        collision++;
    }
    //cout << "hash table overflow\n";
}

int hashtable::searchCustom(string key){
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        else
            index = (hashfunction2(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        probe++;
        if(table[index]->key == key){
            return index;
        }
        probe++;
    }
    return -1;
}

int hashtable::deleteCustom(string key){
    for(int i = 0; i < length;i++){
        int index;
        if(hashfunction == 1)
            index = (hashfunction1(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        else
            index = (hashfunction2(key) + C1 * i * hashfunctionAux(key) + C2 * i * i) % length;
        if(table[index]->key == key){
            table[index] = NULL;
            return index;
        }
    }
    return -1;

}

hashtable::~hashtable(){

}



int main(){
    int length;
    int wordNum;
    cout << "size of table:    " << "number of words\n";
    cin >> length >> wordNum;
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
    'X', 'Y', 'Z'};

    int op,method,hashfunction;;
    string *words;
    words = new string[wordNum];


    while(1){
        cout << "Method: \n";
        cin >> method;
        cout << "hashfunction\n";
        cin >> hashfunction;
        hashtable htc(length,hashfunction);

        if(method == 1) cout << "Chaining Method:\n";
        else if(method == 2) cout << "Double Hashing:\n";
        else cout << "Custom Probing:\n";

        srand(time(NULL));        /// Insert
        int i = 0;
        while(i != wordNum){
            string word = "";
            int flag = 1;
            for (int j = 0; j < 7; j++)
              word = word + alphabet[rand() % 52];
            for(int k = 0; k < i; k++){
                if(words[k] == word){
                    flag = 0;
                    break;
                }
            }
            if(flag){
                words[i] = word;
                htc.insert(word,method);
                i++;
            }
        }
        cout << "collision: " << htc.totalCollision() << endl;


        string word;      ///Search
        int searchkey;
        srand(time(NULL));
        if(method == 1){         ///Chaining method
            int testcount = 1000;
            for(int i = 0; i < testcount; i++){
                word = words[rand() % length];
                searchkey = htc.searchChain(word);
                //if(searchkey) cout << "key found\n";
                //else cout << "key not found\n";
            }
            cout << "avg probing: " << htc.totalProbe()/float(testcount) << endl;
        }

        if(method == 2){        /// Double Hashing
            int testcount = 1000;
            for(int i = 0; i < testcount; i++){
                word = words[rand() % length];
                searchkey = htc.searchDouble(word);
                //if(searchkey >= 0) cout << "key found at " << searchkey << endl;
                //else cout << "key not found\n";
            }
            cout << "avg probing: " << htc.totalProbe()/float(testcount) << endl;
        }

        if(method == 3){      /// Custom probing
            int testcount = 1000;
            for(int i = 0; i < testcount; i++){
                word = words[rand() % length];
                searchkey = htc.searchCustom(word);
                //if(searchkey >= 0) cout << "key found at " << searchkey << endl;
                //else cout << "key not found\n";
            }
            cout << "avg probing: " << htc.totalProbe()/float(testcount) << endl;
        }
        cout <<"For Delete Enter 3\n";
        cin >> op;
        if(op == 3){
            string word;
            int deletekey;
            cin >> word;
            if(method == 1){
                deletekey = htc.deleteChain(word);
                if(deletekey) cout << "key deleted\n";
                else cout << "key not found\n";
            }
            if(method == 2){
                deletekey = htc.deleteDouble(word);
                if(deletekey >= 0) cout << "key deleted at " << deletekey << endl;
                else cout << "key not found\n";
            }
            if(method == 2){
                deletekey = htc.deleteCustom(word);
                if(deletekey >= 0) cout << "key deleted at " << deletekey << endl;
                else cout << "key not found\n";
            }

        }
    }
    return 0;
}
