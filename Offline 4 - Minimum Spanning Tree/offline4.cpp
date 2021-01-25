#include<bits/stdc++.h>
using namespace std;
typedef pair <int,int> pr;
typedef pair <int, pair <int,int> > pqPair;
FILE *fp2=fopen("output.txt","w");

//// set class starts here ////

class disjointSet{
    int setArray[100];

public:
    disjointSet(int v){
        for(int i = 0; i < v; i++)
            setArray[i] = -1;
    }

    int findSet(int data){
        if(setArray[data] == -9999){
            cout << "don't have any set\n";
            return -9999;
        }
        else if(setArray[data] < 0)
            return data;

        else{
            int parent = findSet(setArray[data]);
            setArray[data] = parent;
            return parent;
        }
    }

    void Union(int data1,int data2){
        int parent1,parent2;
        parent1 = findSet(data1);
        parent2 = findSet(data2);

        if (parent1 == parent2)
            cout << "same set\n";

        else if(setArray[parent1] == setArray[parent2]){
            setArray[parent2] = parent1;
            setArray[parent1]--;
        }

        else if (setArray[parent1] < setArray[parent2])
            setArray[parent2] = parent1;

        else if (setArray[parent1] > setArray[parent2])
            setArray[parent1] = parent2;

        return;
    }

    void printSet(int data){
        int tempArray[100],n = 0,temp = 0;
        int parent = findSet(data);
        tempArray[n++] = parent;
        while(temp <= n){
            for (int i = 0; i < 100; i++){
                if(setArray[i] == tempArray[temp]){
                    tempArray[n++] = i;
                }
            }
            temp++;
        }
        cout << "Set elements:" << endl;
        for (int i = 0; i < n; i++)
            cout << tempArray[i] << " ";
            cout << endl;
    }
};


//// sets class ends here ////

//// graph class starts here ////

class Graph
{
	int nVertices, nEdges ;
	vector <pr> * adjList;
	int *key, *parent ;
	priority_queue<pr, vector<pr>, greater<pr> > pq;
	priority_queue <pqPair, vector<pqPair>, greater<pqPair> > PQ;

public:
	Graph();
	~Graph();
	void setnVertices(int n);
	void addEdge(int u, int v, int w);
	int searchEdge(int u,int v);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    void PrimsAlgo(int node);
    void kruskalsAlgo();
    void printGraph();
};


Graph::Graph()
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	parent = 0;
	key = 0;
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	if(adjList!=0) delete[] adjList ; //delete previous list
	adjList = new vector <pr> [nVertices] ;
}


void Graph::addEdge(int u, int v, int w)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
	adjList[u].push_back(make_pair(v,w)) ;
	adjList[v].push_back(make_pair(u,w)) ;
	PQ.push(make_pair(w,make_pair(u,v)));
	this->nEdges++;
	return;
}

int Graph::searchEdge(int u,int v){
    for(int i = 0; i < adjList[u].size(); i++)
        if(adjList[u].at(i).first == v)
            return i;
        else
            return -1;
}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    int index = searchEdge(u,v);
    if(index >= 0){
        adjList[u].erase(adjList[u].begin() + index);
        int index2 = searchEdge(v,u);
        adjList[v].erase(adjList[v].begin() + index2);
        return;
    }
    printf("there is no edge between these two vertices\n");
    return;
}

bool Graph::isEdge(int u, int v)
{
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    for(int i = 0; i < adjList[u].size(); i++){
        if(adjList[u].at(i).first == v)
            return true;
    }
    return false;
}


void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        cout << i << ": ";
        for(int j = 0; j < adjList[i].size();j++){
            cout << adjList[i].at(j).first << "(" << adjList[i].at(j).second << "), " ;
        }
        cout << "\n";
    }
}

void Graph::PrimsAlgo(int node){
    bool *inMST;
    key = new int[nVertices];
    parent = new int[nVertices];
    inMST = new bool[nVertices];
    for(int i = 0; i < nVertices; i++){
        key[i] = 999999;
        parent[i] = -1;
        inMST[i] = false;
    }
    int root = node;
    key[root] = 0;
    pq.push(make_pair(key[root],root));
    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        if(inMST[u])
            continue;
        inMST[u] = true;
        for(int i = 0; i < adjList[u].size(); i++){
            int v = adjList[u].at(i).first;
            int w = adjList[u].at(i).second;
            if(inMST[v] == false && key[v] > w){
                //cout << u << ": " << v << " " << w << endl;
                parent[v] = u;
                key[v] = w;
                pq.push(make_pair(key[v],v));
            }
        }
    }
    fprintf(fp2,"Prim's Algorithm:\n");
    fprintf(fp2,"Root node = %d\n",root);
    for(int i = 0; i < nVertices; i++){
        if(i == root)
            continue;
        fprintf(fp2,"%d %d\n",parent[i],i);
    }
}

void Graph::kruskalsAlgo(){
    vector <pr> tree;
    disjointSet ds(nVertices);
    int length = PQ.size();
    int mst_weight = 0;

    for(int i = 0; i < length; i++){
        int u = PQ.top().second.first;
        int v = PQ.top().second.second;
        //cout << u << " " << v << " " << ds.findSet(u) << " " << ds.findSet(v) << endl;
        if(ds.findSet(u) != ds.findSet(v)){
            mst_weight += PQ.top().first;
            tree.push_back(make_pair(u,v));
            ds.Union(u,v);
        }
        PQ.pop();
    }
    fprintf(fp2,"%d\n",mst_weight);
    fprintf(fp2,"Kruskal's algorithm:\n");
    for(int i = 0; i < tree.size(); i++)
        fprintf(fp2,"%d %d\n",tree[i].first,tree[i].second);
}

Graph::~Graph()
{
    if(key) delete[] key;
    if(parent) delete[] parent;
    if(adjList) delete[] adjList;
    printf("destructor called\n");
}


//// graph class starts here ////


int main(){
    int n,m,u,v,s,d,w;
    Graph g;

    FILE *fp1=fopen("input1.txt","r");

    fscanf(fp1,"%d",&n);
    fscanf(fp1,"%d",&m);

    g.setnVertices(n);

    for (int i = 1; i <= m; i++){
        fscanf(fp1,"%d",&u);
        fscanf(fp1,"%d",&v);
        fscanf(fp1,"%d",&w);
        g.addEdge(u,v,w);
    }
    //g.printGraph();
    int node;
    cout << "Root Node for Prim's\n";
    cin >> node;
    g.kruskalsAlgo();
    g.PrimsAlgo(node);

    return 0;
}

