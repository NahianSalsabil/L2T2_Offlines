#include<bits/stdc++.h>
#define NULL_value -1
using namespace std;


typedef pair <int,int> pr;

//// array list class starts here ////

class Edge{
    int v;
    double w;
public:
    Edge();
    ~Edge();
    void setV(int v);
    void setW(double w);
    int getV();
    double getw();
};

Edge::Edge(){

}

Edge::~Edge(){

}

void Edge::setV(int v){
    this->v = v;
}

void Edge::setW(double w){
    this->w = w;
}
int Edge::getV(){
    return v;
}

double Edge::getw(){
    return w;
}


//// Edge class ends here

//// graph class starts here ////

class Graph
{
    Edge edge;
	int nVertices, nEdges ;
	bool directed ;

	int **distance ;
	int **parent,*h,*dij,*dij_par;
	vector <vector <Edge> > adjList;
	vector <vector <Edge> > tempadjList;
	priority_queue<pr, vector<pr>, greater<pr> > pq;
	stack <int> st;

public:
	Graph(bool dir = true);
	~Graph();
	void setnVertices(int n);
	bool addEdge(int u, int v,double w);
	void removeEdge(int u, int v);
	int getIndex(int u,int v);
	bool isEdge(int u,int v);
	Edge* searchEdge(int u,int v);
    void reweightEdge(int u,int v,double w);
    double getWeight(int u,int v);
    void printGraph();
    void Dijkstra(int n);
    bool BellmanFord();
    void floydWarshall();
    void johnsonsAlgo();
    void printShortestpath(int u,int v);
    double printShortestpathWeight(int u,int v);
    void printDistanceMatrix();
    void printPredeccesorMatrix();
    void cleanSPInfo();
   // void GraphLand(int x,int y,int u,int v);
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	directed = dir ; //set direction of the graph
	parent = 0;
	h = 0;
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
	parent = new int *[nVertices];
	for(int i = 0; i < nVertices; i++){
        parent[i] = new int [nVertices];
	}
	distance = new int* [nVertices];
	for(int i = 0; i < nVertices; i++){
        distance[i] = new int [nVertices];
	}

    h = new int[nVertices];
}


bool Graph::addEdge(int u, int v,double w)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return false; //vertex out of range
    if(searchEdge(u,v) != nullptr) return false;
    edge.setV(v);
    edge.setW(w);
    adjList[u].push_back(edge);
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

void Graph::reweightEdge(int u,int v,double w){
    Edge *e;
    int index;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    e = searchEdge(u,v);
    if(e != nullptr)
        e->setW(w);

    else
        addEdge(u,v,w);
}

double Graph::getWeight(int u,int v){
    Edge e;
    int index;
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return 999999;
    index = getIndex(u,v);
    if(index >= 0){
        e = adjList[u].at(index);
        return e.getw();
    }
    else
        return 999999;
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

void Graph::printGraph()
{
    Edge e;
//    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    cout << "Graph:\n";
    for(int i = 0; i < nVertices; i++)
    {
        cout << i+1 << " : ";
        for(int j = 0; j < adjList[i].size();j++){
            e = adjList[i].at(j);
            cout << e.getV()+1 << "(" << e.getw() << ") --> " ;
        }
        cout << "\n";
    }
}

void Graph::printShortestpath(int u,int v){
    if(parent[u][v] == NULL_value)
        return;
    int x = v;
    while(x != u){
        st.push(x);
        x = parent[u][x];
    }
    cout << "Path: " << u+1;
    int temp = u;
    while(!st.empty()){
        x = st.top();
        st.pop();
        cout << " --> " << x+1 << "(" << distance[temp][x] << ")";
        temp = x;
    }
    cout << endl;
}

double Graph::printShortestpathWeight(int u,int v){
    return distance[u][v];
}

void Graph::printDistanceMatrix(){
    cout << "Distance Matrix\n";
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            if(distance[i][j] == 999999)
                cout << "INF" << "\t";
            else
                cout << distance[i][j] << "\t";
        }
        cout << endl;
    }
}
void Graph::printPredeccesorMatrix(){
    cout << "Predecessor Matrix\n";
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            if(parent[i][j] == NULL_value)
                cout << "NIL" << "\t";
            else
                cout << parent[i][j] + 1 << "\t";
        }
        cout << endl;
    }
}

void Graph::cleanSPInfo(){
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            distance[i][j] = 999999;
        }
    }
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            parent[i][j] = -1;
        }
    }
    cout << "APSP matrices cleared\n";
}

void Graph::Dijkstra(int s){
    dij = new int[nVertices];
    dij_par = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        dij[i] = 999999;
        dij_par[i] = -1;
    }
    dij[s] = 0;

    pq.push(make_pair(0,s));
    distance[s] = 0;
    while(!pq.empty()){
        int dis = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        for(int i = 0; i < adjList[u].size(); i++){
            int v = adjList[u].at(i).getV();
            if(isEdge(u,v)){
                if(dij[v] > dij[u] + abs(adjList[u].at(i).getw())){
                    dij[v] = dij[u] + abs(adjList[u].at(i).getw());
                    pq.push(make_pair(dij[v],v));
                    dij_par[v] = u;
                }
            }
        }
    }
}

void Graph::floydWarshall(){

    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            if(i == j){
                distance[i][j] = 0;
                parent[i][j] = -1;
            }
            else if(getWeight(i,j) == 999999){
                distance[i][j] = 999999;
                parent[i][j] = -1;
            }
            else {
                distance[i][j] = getWeight(i,j);
                parent[i][j] = i;
            }
        }
    }

    for(int k = 0; k < nVertices; k++){
        for(int i = 0; i < nVertices; i++){
            for(int j = 0; j < nVertices; j++){
                if(distance[i][k] + distance[k][j] < distance[i][j]){
                    distance[i][j] = distance[i][k] + distance[k][j];
                    parent[i][j] = parent[k][j];
                }
            }
        }
    }
    cout << "Floyd-Warshall algorithm implemented\n";
}

void Graph::johnsonsAlgo(){
    Graph temp_g(true);
    temp_g.setnVertices(nVertices + 1);
    for(int i = 0; i < temp_g.nVertices - 1; i++){         /// copy graph to a new graph
        for(int j = 0; j < adjList[i].size(); j++){
            temp_g.addEdge(i,adjList[i].at(j).getV(),adjList[i].at(j).getw());
        }
    }

    for(int i = 0; i < temp_g.nVertices - 1; i++) /// add s vertex with weight 0 to all the vertex
        temp_g.addEdge(temp_g.nVertices - 1,i,0);

    //temp_g.printGraph();
    bool isNeg;
    isNeg = temp_g.BellmanFord(); /// Bellman Ford
    if(isNeg){
        cout << "Graph contain negative cycle\n";
        return;
    }
    else{
        int tempW;
        for(int i = 0; i < temp_g.nVertices - 1; i++){      /// reweight graph
            for(int j = 0; j < adjList[i].size(); j++){
                tempW = adjList[i].at(j).getw() + temp_g.h[i] - temp_g.h[adjList[i].at(j).getV()];
                temp_g.reweightEdge(i,adjList[i].at(j).getV(),tempW);

            }
        }
        //temp_g.printGraph();
        for(int i = 0; i < temp_g.nVertices - 1; i++){
            temp_g.Dijkstra(i);
            for(int j = 0; j < temp_g.nVertices - 1; j++){
                distance[i][j] = temp_g.dij[j] + temp_g.h[j] - temp_g.h[i];
                parent[i][j] = temp_g.dij_par[j];
            }
        }

    }
    cout << "Johnson’s algorithm implemented\n";
}


bool Graph::BellmanFord(){
    for(int i = 0; i < nVertices; i++){
        h[i] = 999999;
    }
    h[nVertices - 1] = 0;
    for(int i = 0; i < nVertices; i++){
        for(int j = 0; j < nVertices; j++){
            int u = j;
            for(int k = 0; k < adjList[j].size(); k++){
                int v = adjList[u].at(k).getV();
                if(isEdge(u,v)){
                    if(h[v] > h[u] + adjList[u].at(k).getw()){
                        h[v] = h[u] + adjList[u].at(k).getw();
                    }
                }
            }
        }
    }

    for(int j = 0; j < nVertices; j++){
        int u = j;
        for(int k = 0; k < adjList[j].size(); k++){
            int v = adjList[u].at(k).getV();
            if(isEdge(u,v)){
                if(h[v] > h[u] + adjList[u].at(k).getw()){
                    return true;

                }
            }
        }
    }
    //cout << "bellman ford done\n";
    return false;
}

Graph::~Graph()
{
    //write your destructor here
    if(distance){
        for(int i = 0; i < nVertices; i++){
            delete[] distance[i];
        }
        delete[] distance;
    }
    if(parent){
        for(int i = 0; i < nVertices; i++){
            delete[] parent[i];
        }
        delete[] parent;
    }

    for(int i = 0; i < nVertices; i++){
            for(int j = 0 ; j < adjList[i].size(); j++){

            }
    }
    if(h) delete [] h;
    printf("destructor called\n");
}


//// graph class ends here ////



int main(){
    int n,m,u,v,s,d;
    double w;
    Graph g(true);
    FILE *fp1=fopen("input1.txt","r");


    fscanf(fp1,"%d",&n);
    fscanf(fp1,"%d",&m);

    g.setnVertices(n);
    bool edge;
    for (int i = 1; i <= m; i++){
        fscanf(fp1,"%d",&u);
        fscanf(fp1,"%d",&v);
        fscanf(fp1,"%lf",&w);
        edge = g.addEdge(u-1,v-1,w);
    }
    cout << "Graph Created.\n";

    int c;
    while(true){
        cout << "1.clear.  2.Floyd-Warshall Algo.  3.Johnsons Algo.  4.Query.  5.Print Graph.  6.Print distances.  7.Print Parents.\n" ;
        cin >> c;

        if(c == 1){
            g.cleanSPInfo();
        }
        else if (c == 2) /// floydwarshall
        {
            g.floydWarshall();
        }
        else if (c == 3) /// johnsons
        {
            g.johnsonsAlgo();
        }
        else if (c == 4){
            cin >> u >> v;
            double weight;
            weight = g.printShortestpathWeight(u-1,v-1);
            cout << "Shortest Path Weight: " << weight << endl;
            g.printShortestpath(u-1,v-1);
        }
        else if (c == 5) /// print graph
        {
            g.printGraph();
        }
        else if (c == 6) /// print distance
        {
            g.printDistanceMatrix();
        }
        else if (c == 7) /// print parent
        {
            g.printPredeccesorMatrix();
        }
        else{
            break;
        }
    }


    return 0;
}

