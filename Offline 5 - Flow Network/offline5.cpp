#include<bits/stdc++.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3
using namespace std;


typedef pair <int,int> pr;

//// array list class starts here ////

class Edge{
    int v,c,f;
public:
    Edge();
    ~Edge();
    void setV(int v);
    void setF(int f);
    void setC(int c);
    int getV();
    int getc();
    int getF();
};

Edge::Edge(){

}

Edge::~Edge(){

}

void Edge::setV(int v){
    this->v = v;
}

void Edge::setC(int c){
    this->c = c;
}
void Edge::setF(int f){
    this->f = f;
}
int Edge::getV(){
    return v;
}

int Edge::getc(){
    return c;
}
int Edge::getF(){
    return f;
}


//// Edge class ends here

//// graph class starts here ////

class Graph
{
    Edge edge;
	int nVertices, nEdges ;
	bool directed,*visited ;
	int *distance,*parent,*color_info ;
	vector <vector <Edge> > adjList;
	queue <int> q;

public:
	Graph(bool dir = true);
	~Graph();
	void setnVertices(int n);
	bool addEdge(int u, int v, int c);
	void removeEdge(int u, int v);
	int getIndex(int u,int v);
	bool isEdge(int u,int v);
	Edge* searchEdge(int u,int v);
    void reweightEdge(int u,int v,int c);
    double getWeight(int u,int v);
    bool bfs(int source,int t);
    bool dfs(int source);
    int maxFlow(int s,int t);
    void printFlowNetwork();
    void printMatching();
    void fixFlowNetwork();
    void bipartiteMatching();
    void printGraph();
    void printFlowInFile(int max_flow);
    void printMatchingInFile(int max_flow);
   // void GraphLand(int x,int y,int u,int v);
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	directed = dir ; //set direction of the graph
	parent = 0;
	color_info = 0;
	distance = 0;
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	for(int i = 0; i < nVertices; i++){
        vector<Edge> line;
        adjList.push_back(line);
	}

}


bool Graph::addEdge(int u, int v,int c)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return false; //vertex out of range
    if(searchEdge(u,v) != nullptr) return false;
    edge.setV(v);
    edge.setC(c);
    edge.setF(0);
    adjList[u].push_back(edge);
    if(!directed){
        edge.setV(u);
        edge.setC(c);
        edge.setF(0);
        adjList[v].push_back(edge);
    }
    this->nEdges++;
	return true;

}


void Graph::removeEdge(int u, int v)
{
    Edge e;
    int index;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    index = getIndex(u,v);
    if(index >= 0){
        adjList[u].erase(adjList[u].begin()+index);
        this->nEdges--;
    }
    else
        cout << "No such edge\n";

}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    Edge e;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    for(int i = 0; i < adjList[u].size(); i++){
        e = adjList[u].at(i);
        if(e.getV() == v)
            return true;
    }
    return false;
}

Edge* Graph::searchEdge(int u,int v){

    for(int i = 0; i < adjList[u].size();i++){
        if(adjList[u].at(i).getV() == v)
            return &adjList[u].at(i);
    }
    return nullptr;
}

void Graph::reweightEdge(int u,int v,int c){
    Edge *e;
    int index;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    e = searchEdge(u,v);
    if(e != nullptr)
        e->setC(c);

    else
        addEdge(u,v,c);
}

double Graph::getWeight(int u,int v){
    Edge e;
    int index;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return INFINITY;
    index = getIndex(u,v);
    if(index >= 0){
        e = adjList[u].at(index);
        return e.getc();
    }
    else
        return INFINITY;
}


 int Graph::getIndex(int u,int v){
    Edge e;
    for(int i = 0; i < adjList[u].size(); i++){
        e = adjList[u].at(i);
        if(e.getV() == v)
            return i;
    }
    return -1;
}

bool Graph::bfs(int source,int t)
{
    //complete this function
    //initialize BFS variables for all n vertices first
    if(source < 0 || source >= nVertices) return false;
    color_info = new int[nVertices];
    distance = new int[nVertices];
    parent = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        color_info[i] = WHITE;
        distance[i] = INFINITY;
        parent[i] = -1;
    }
    q.push(source);
    distance[source] = 0;
    parent[source] = -1;

    while(!q.empty()){
        int front = q.front();
        q.pop();
        color_info[front] = GREY;
        for(int i = 0; i < nVertices; i++){
            if(isEdge(front,i) && getWeight(front,i) > 0 &&  color_info[i] == WHITE){
                color_info[i] = GREY;
                distance[i] = distance[front] + 1;
                parent[i] = front;
                q.push(i);
            }
        }
        color_info[front] = BLACK;
    }
    return (color_info[t] == BLACK);

}

int Graph::maxFlow(int s,int t){
    Graph temp_g(true);
    temp_g.setnVertices(nVertices);
    for(int i = 0; i < temp_g.nVertices; i++){         /// copy graph to a new graph
        for(int j = 0; j < adjList[i].size(); j++){
            temp_g.addEdge(i,adjList[i].at(j).getV(),adjList[i].at(j).getc());
        }
    }
    int max_flow = 0,u;
    while(temp_g.bfs(s,t)){
        int path_flow = 999999;
        for(int v = t; v != s; v = temp_g.parent[v]){   /// find max path flow
            u = temp_g.parent[v];
            int cap;
            for(int i = 0; i < temp_g.adjList[u].size(); i++){
                if(temp_g.adjList[u].at(i).getV() == v){
                    cap = temp_g.adjList[u].at(i).getc();
                }

            }
            path_flow = min(path_flow,cap);
        }
        for (int v = t; v != s; v = temp_g.parent[v]){    ///  residual network
            u = temp_g.parent[v];
            int isedge = 1,nu,nv;
            if(!isEdge(u,v)) {nu = u; nv = v; isedge = 0;}
            for(int i = 0; i < temp_g.adjList[u].size(); i++){
                if(temp_g.adjList[u].at(i).getV() == v){
                    int rcap = temp_g.adjList[u].at(i).getc() - path_flow;
                    temp_g.adjList[u].at(i).setC(rcap);
                    break;
                }
            }
            if(!isedge){
                int uvIndex = getIndex(nv,nu);
                int new_flow = adjList[nv].at(uvIndex).getF() - path_flow;
                adjList[nv].at(uvIndex).setF(new_flow);
            }
            for(int i = 0; i < adjList[u].size(); i++){    /// flow network

                if(adjList[u].at(i).getV() == v){
                    int new_flow = adjList[u].at(i).getF() + path_flow;
                    adjList[u].at(i).setF(new_flow);
                }
            }
            int flag = 0;
            for(int i = 0; i < temp_g.adjList[v].size(); i++){ /// residual network
                if(temp_g.adjList[v].at(i).getV() == u){
                    int rcap = temp_g.adjList[v].at(i).getc() + path_flow;
                    temp_g.adjList[v].at(i).setC(rcap);
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                edge.setV(u);
                edge.setC(path_flow);
                temp_g.adjList[v].push_back(edge);
            }
        }
        fixFlowNetwork();
        max_flow += path_flow;
    }
    return max_flow;
}

void Graph::fixFlowNetwork(){
    Edge e;
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            if(e.getF() > e.getc()){
                int index = getIndex(e.getV(),i);
                adjList[i].at(j).setF(e.getF() - adjList[e.getV()].at(index).getF());
                adjList[e.getV()].at(index).setF(0);
            }
        }
    }
}

bool Graph::dfs(int source){

    int u = source;
    for(int i = 0; i < nVertices; i++){
        if(isEdge(u,i)){
            if(!visited[i]){
                visited[i] = true;
                if(color_info[u] == WHITE)
                    color_info[i] = BLACK;
                else if(color_info[u] == BLACK)
                    color_info[i] = WHITE;
                if(!dfs(i))
                    return false;
            }
            else if(color_info[u] == color_info[i]){
                return false;
            }
        }
    }
    return true;
}

void Graph::bipartiteMatching(){
    color_info = new int[nVertices];
    visited = new bool[nVertices];
    for(int i = 0; i < nVertices; i++){
        color_info[i] = WHITE;
        visited[i] = false;
    }
    bool bipartite = dfs(0);
    if(!bipartite){
        cout << "The graph is not bipartite\n";
        ofstream ofile;
        ofile.open("output.txt");
        ofile << "The graph is not bipartite\n";
    }

    else{
        setnVertices(nVertices + 2);   /// make a new graph adding source and sink
        this->directed = true;
        int s = nVertices - 2;
        int t = nVertices - 1;
        for(int i = 0; i < nVertices - 2; i++){
            if(color_info[i] == WHITE){
                addEdge(s,i,1);
            }
            else
                addEdge(i,t,1);
        }
        for(int i = 0; i < nVertices; i++){
            if(isEdge(s,i)){
                for(int j = 0; j < adjList[i].size(); j++){
                    auto iter = adjList.begin();
                    int v = adjList[i].at(j).getV();
                    advance(iter,v);
                    for(int k = 0; k < adjList[v].size(); k++){
                        if(adjList[v].at(k).getV() == i){
                            iter->erase(iter->begin() + k);
                            k--;
                        }
                    }

                }
            }
        }
        int max_flow = maxFlow(s,t);
        cout << max_flow << endl;
        printMatchingInFile(max_flow);
    }

}

void Graph::printFlowInFile(int max_flow){
    ofstream ofile;
    ofile.open("output.txt");
    ofile << max_flow << endl;
    Edge e;
    for (int i = 0; i < nVertices; i++){
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            ofile << i << " " << e.getV() << " " << e.getF() << "/" << e.getc() << endl;
        }
    }
}

void Graph::printMatchingInFile(int max_flow){
    ofstream ofile;
    ofile.open("output.txt");
    ofile << max_flow << endl;
    Edge e;
    for (int i = 0; i < nVertices - 2; i++){
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            if(e.getV() != (nVertices - 1) && e.getF() == 1){
                ofile << i << " " << e.getV() << endl;
            }
        }
    }
}

void Graph::printMatching(){
    for(int i = 0; i < nVertices - 2; i++){
        for(int j = 0; j < adjList[i].size(); j++){
            if(adjList[i].at(j).getV() != (nVertices - 1) && adjList[i].at(j).getF() == 1){
                cout << i << " " << adjList[i].at(j).getV() << endl;
            }
        }
    }
}

void Graph::printFlowNetwork(){
    Edge e;
    for (int i = 0; i < nVertices; i++){
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            cout << i << " " << e.getV() << " " << e.getF() << "/" << e.getc() << endl;
        }
    }

}

void Graph::printGraph()
{
    Edge e;
    cout << "Graph:\n";
    for(int i = 0; i < nVertices; i++)
    {
        cout << i << " : ";
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            cout << e.getV() << "(" << e.getc() << ") --> " ;
        }
        cout << "\n";
    }
}


Graph::~Graph()
{
    //write your destructor here
    if(distance){
        delete[] distance;
    }
    if(parent){
        delete[] parent;
    }
    printf("destructor called\n");
}


//// graph class ends here ////



int main(){
    int n,m,u,v,c,s,t,choice;
    bool dir;
    printf("1. directed graph   2. undirected graph\n");
    scanf("%d",&choice);

    if(choice == 1) dir = true;
    else if(choice == 2) dir = false;
    Graph g(dir);

    FILE *fp1=fopen("in1.txt","r");
    fscanf(fp1,"%d",&n);
    fscanf(fp1,"%d",&m);

    g.setnVertices(n);
    bool edge;
    if(choice == 1) {
        for (int i = 1; i <= m; i++){
            fscanf(fp1,"%d",&u);
            fscanf(fp1,"%d",&v);
            fscanf(fp1,"%d",&c);
            edge = g.addEdge(u,v,c);
        }
        fscanf(fp1,"%d",&s);
        fscanf(fp1,"%d",&t);
    }
    else if (choice == 2){
        for (int i = 1; i <= m; i++){
            fscanf(fp1,"%d",&u);
            fscanf(fp1,"%d",&v);
            edge = g.addEdge(u,v,1);
        }
    }
    cout << "Graph Created.\n";


    int opt;
    while(true){
        cout << "1.bfs.  2.Maximum Flow  3.Bipartite  4.Print Graph.\n" ;
        cin >> opt;

        if(opt == 1){
            g.bfs(s,t);
        }
        else if (opt == 2) /// max flow
        {
            int maximum_flow;
            maximum_flow = g.maxFlow(s,t);
            cout << maximum_flow << endl;
            g.printFlowNetwork();
            g.printFlowInFile(maximum_flow);
        }
        else if (opt == 3) /// bipartite
        {
            g.bipartiteMatching();
            g.printMatching();
        }
        else if (opt == 4)  /// print graph
        {
            g.printGraph();
        }
        else{
            break;
        }
    }


    return 0;
}

