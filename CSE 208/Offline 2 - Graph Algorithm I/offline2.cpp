#include<bits/stdc++.h>
using namespace std;
typedef pair <int,int> pr;
FILE *fp2=fopen("output.txt","w");

//// array list class starts here ////

class ArrayList
{
	int * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(int item) ;
    void insertItem(int item) ;
	void removeItem(int item) ;
	void removeItemAt(int item);
	int getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new int[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(int newitem)
{
	int * tempList ;
	if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;  // doubling memory when array is full
		tempList = new int[listMaxSize] ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        delete[] list ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};

	list[length] = newitem ; //store new item
	length++ ;
}

int ArrayList::searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL;
}

void ArrayList::removeItemAt(int position) //do not preserve order of items
{
	if ( position < 0 || position >= length ) return ; //nothing to remove
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL ) return ; //nothing to remove
	removeItemAt(position) ;
}


int ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return NULL ;
	return list[position] ;
}

int ArrayList::getLength()
{
	return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//// Array list class ends here ////

//// graph class starts here ////

class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList,* weightList ;
	int *distance ;
	//int *color_info;
	int *parent;
	priority_queue<pr, vector<pr>, greater<pr> > pq;
	stack <int> st;
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation

public:
	Graph(bool dir = true);
	~Graph();
	void setnVertices(int n);
	void setnEdges(int m);
	void addEdge(int u, int v);
	void addWeight(int u,int v,int w);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getInDegree(int u);
    int getOutDegree(int u);
    void printAdjVertices(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
    void Dijkstra(int s,int d);
    bool BellmanFord(int s,int d);

};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	weightList = 0;
	directed = dir ; //set direction of the graph
	parent = 0;
	//color_info = 0;
	distance = 0;
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	if(adjList!=0) delete[] adjList ; //delete previous list
	adjList = new ArrayList[nVertices] ;
}
void Graph::setnEdges(int m)
{
	this->nEdges = m ;
	if(weightList!=0) delete[] weightList ; //delete previous list
	weightList = new ArrayList[nEdges] ;

}

void Graph::addEdge(int u, int v)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
    //this->nEdges++ ;
	adjList[u].insertItem(v) ;
	return;
}

void Graph::addWeight(int u,int v,int w){

    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return;
    weightList[u].insertItem(w);
    return;
}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    if(adjList[u].searchItem(v) >= 0){
        adjList[u].removeItem(v);
        this->nEdges--;
        if(!directed){
            adjList[v].removeItem(u);
            this->nEdges--;
        }
    }
    else
        printf("there is no edge between these two vertices\n");

}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u >= 0 && u < nVertices && v >= 0 && v < nVertices){
        if(adjList[u].searchItem(v) >= 0)
            return true;
        return false;
    }
    else{
        printf("No such vertices\n");
        return false;
    }

}

int Graph::getOutDegree(int u)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return -1;
    int degree = 0;
    for(int i = 0; i < nVertices; i++){
        if(adjList[u].searchItem(i) >= 0)
            degree++;
    }
    return degree;
}

int Graph::getInDegree(int u)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return -1;
    int degree = 0;
    for(int i = 0; i < nVertices; i++){
        if(i == u)
            continue;
        if(adjList[i].searchItem(u) >= 0)
            degree++;
    }
    return degree;


}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u < 0 || u >= nVertices) return;

    for(int i = 0; i < nVertices; i++){
        if(adjList[u].searchItem(i) >= 0)
            printf("%d: %d\n",u,i);
    }
    return;
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) {
        printf("No such vertices\n");
        return false;
    }
    for(int i = 0; i < nVertices; i++){
        if(adjList[u].searchItem(i) >= 0 && adjList[v].searchItem(i) >= 0)
            return true;
    }
    return false;


}

void Graph::Dijkstra(int s,int d){
    distance = new int[nVertices];
    parent = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        distance[i] = 999999;
        parent[i] = NULL;
    }
    pq.push(make_pair(0,s));
    distance[s] = 0;
    while(!pq.empty()){
        int dis = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        for(int i = 0; i < adjList[u].getLength(); i++){
            int v = adjList[u].getItem(i);
            if(isEdge(u,v)){
                if(distance[v] > distance[u] + abs(weightList[u].getItem(i))){
                    //cout << u << " " << v << " " << k << " " << weightList[4].getItem(0) << endl;
                    //cout << "distance u: " << distance[u] << " " << weightList[u].getItem(i) << "\n";
                    distance[v] = distance[u] + abs(weightList[u].getItem(i));
                    //cout << "u: " << u << " v: " << v << " d: " << distance[v] << " " << endl;
                    pq.push(make_pair(distance[v],v));
                    parent[v] = u;
                }
            }
        }
    }
    fprintf(fp2,"Dijkstra Algorithm:\n");
    fprintf(fp2,"%d\n",distance[d]);

    int x = d;
    while(x != s){
        st.push(x);
        x = parent[x];
    }
    fprintf(fp2,"%d",s);
    while(!st.empty()){
        fprintf(fp2," -> %d",st.top());
        st.pop();
    }
    fprintf(fp2,"\n");


}

bool Graph::BellmanFord(int s,int d){
    // Initialization
    distance = new int[nVertices];
    parent = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        distance[i] = 999999;
        parent[i] = NULL;
    }
    distance[s] = 0;
//    for(int i = 0; i < nVertices; i++){
//        //distance[i] = 999999;
//        cout << parent[i] << endl;
//    }

    for(int i = 0; i < nVertices - 1; i++){
        for(int j = 0; j < nVertices; j++){
            int u = j;
            for(int k = 0; k < adjList[j].getLength(); k++){
                int v = adjList[j].getItem(k);
                if(isEdge(u,v)){
                    if(distance[v] > distance[u] + weightList[u].getItem(k)){
//                        cout << u << " " << v << " " << k << " " << weightList[4].getItem(0) << endl;
//                        cout << "distance u: " << distance[u] << " " << weightList[u].getItem(k) << "\n";
                        distance[v] = distance[u] + weightList[u].getItem(k);
                        //cout << "u: " << u << " v: " << v << " d: " << distance[v] << " " << endl;
                        parent[v] = u;
                    }
                }
            }
        }
        //cout << "\n";
    }
    for(int i = 0; i < nVertices; i++){
        int u = i;
        for(int j = 0; j < adjList[i].getLength(); j++){
            int v = adjList[u].getItem(j);
            if(isEdge(u,v)){
                if(distance[v] > distance[u] + weightList[u].getItem(j)){
//                    cout << "distance u: " << distance[u] << " " << weightList[u].getItem(j) << "\n";
//                    cout << "u: " << u << " v: " << v << " d: " << distance[v] << " " << endl;
                    return false;
                }

            }
        }
    }
    //cout << "yo\n";
    fprintf(fp2,"Bellman Ford Algorithm:\n");
    fprintf(fp2,"%d\n",distance[d]);
    int x = d;
    while(x != s){
        st.push(x);
        x = parent[x];
    }
    fprintf(fp2,"%d",s);
    while(!st.empty()){
        fprintf(fp2," -> %d",st.top());
        st.pop();
    }
    fprintf(fp2,"\n\n");
    return true;
}

void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i);
        for(int j=0; j<adjList[i].getLength();j++)
        {
            printf(" %d", adjList[i].getItem(j));
        }
        printf("\n");
    }
    cout << "\n";
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i);
        for(int j=0; j<weightList[i].getLength();j++)
        {
            printf(" %d", weightList[i].getItem(j));
        }
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(distance) delete[] distance;
    //if(color_info) delete[] color_info;
    if(parent) delete[] parent;
    if(adjList) delete[] adjList;
    printf("destructor called\n");
}


//// graph class starts here ////



int main(){
    int n,m,u,v,s,d,weight;
    Graph g(true);

    FILE *fp1=fopen("input.txt","r");


    fscanf(fp1,"%d",&n);
    fscanf(fp1,"%d",&m);

//    cout << n << endl;
//    cout << m << endl;
    g.setnVertices(n);
    g.setnEdges(m);

    for (int i = 1; i <= m; i++){
        fscanf(fp1,"%d",&u);
        fscanf(fp1,"%d",&v);
        g.addEdge(u,v);
        fscanf(fp1,"%d",&weight);
        g.addWeight(u,v,weight);
    }

    fscanf(fp1,"%d",&s);
    fscanf(fp1,"%d",&d);
    //cout << s << " " << d << endl;


   // g.printGraph();



    bool flag;
    flag = g.BellmanFord(s,d);
    if(!flag)
        cout << "negative edge cycle detected\n";

    g.Dijkstra(s,d);


    return 0;
}
