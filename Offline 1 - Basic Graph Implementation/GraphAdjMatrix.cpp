#include<bits/stdc++.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3
using namespace std;

class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue();
    ~Queue();
    void enqueue(int item); //insert item in the queue
    int dequeue(); //returns the item according to FIFO
    bool empty(); //return true if Queue is empty
};

Queue::Queue()
{
    queueInitSize = 2 ;
    queueMaxSize = queueInitSize;
    data = new int[queueMaxSize] ; //allocate initial memory
    length = 0 ;
    front = 0;
    rear = 0;
}


void Queue::enqueue(int item)
{
	if (length == queueMaxSize)
	{
		int * tempData ;
		//allocate new memory space for tempList
		queueMaxSize = 2 * queueMaxSize ;
		tempData = new int[queueMaxSize] ;
		int i, j;
		j = 0;
		for( i = rear; i < length ; i++ )
		{
			tempData[j++] = data[i] ; //copy items from rear
		}
		for( i = 0; i < rear ; i++ )
		{
			tempData[j++] = data[i] ; //copy items before rear
		}
		rear = 0 ;
		front = length ;
		delete[] data ; //free the memory allocated before
		data = tempData ; //make list to point to new memory
	}

	data[front] = item ; //store new item
	front = (front + 1) % queueMaxSize ;
	length++ ;
}


bool Queue::empty()
{
	if(length == 0) return true ;
	else return false ;
}


int Queue::dequeue()
{
	if(length == 0) return NULL_VALUE ;
	int item = data[rear] ;
	rear = (rear + 1) % queueMaxSize ;  // circular queue implementation
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; //deallocate memory
    data = 0; //set to NULL
}

//****************Queue class ends here************************


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	int *parent;
	int *distance ;
	int *color_info;
	int ** matrix ; //adjacency matrix to store the graph
	Queue q;
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation

public:
	Graph(bool dir = false);
	~Graph();
	void setnVertices(int n);
	bool addEdge(int u, int v);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getInDegree(int u);
    int getOutDegree(int u);
    void printAdjVertices(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
	void bfs(int source); //will run bfs in the graph
	void dfs(int source); //will run dfs in the graph
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	matrix = 0 ;
	directed = dir ; //set direction of the graph
	color_info = 0;
	parent = 0;
	distance = 0;
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;

	//allocate space for the matrix
    matrix = new int*[nVertices];

    for(int i=0;i<nVertices;i++)
    {
        matrix[i] = new int[nVertices];
        for(int j=0;j<nVertices;j++)
            matrix[i][j] = 0; //initialize the matrix cells to 0
    }

}

bool Graph::addEdge(int u, int v)
{
    //write your code here
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    if(!matrix[u][v]){
        matrix[u][v] = 1;
        this->nEdges++;
        if(!directed){
            matrix[v][u] = 1;
            this->nEdges++;
        }
        return true;
    }
    return false;

}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    if(matrix[u][v]){
        matrix[u][v] = 0;
        this->nEdges--;
        if(!directed){
            matrix[v][u] = 0;
            this->nEdges--;
        }
    }
    else
        printf("there is no edge between these two vertices\n");


}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u >= 0 || u < nVertices || v >= 0 || v < nVertices){
        if(matrix[u][v])
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
        if(i == u)
            continue;
        if(matrix[u][i])
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
        if(matrix[i][u])
        degree++;
    }
    return degree;

}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u < 0 || u >= nVertices) return;

    for(int i = 0; i < nVertices; i++){
        if(matrix[u][i])
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
        if(matrix[u][i] && matrix[v][i])
            return true;
    }
    return false;

}

void Graph::bfs(int source)
{
    //write this function
    if(source < 0 || source >= nVertices) return;
    color_info = new int[nVertices];
    parent = new int[nVertices];
    distance = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        color_info[i] = WHITE ;
        distance[i] = INFINITY;
        parent[i] = -1;
    }

    q.enqueue(source);
    distance[source] = 0;
    parent[source] = -1;

    while(!q.empty()){
        int front = q.dequeue();
        //printf("%d ",front);
        color_info[front] = GREY;
        for(int i = 0; i < nVertices; i++){
            if(isEdge(front,i) && color_info[i] == WHITE){
                color_info[i] = GREY;
                parent[i] = front;
                distance[i] = distance[front] + 1;
                q.enqueue(i);
            }
        }
        color_info[front] = BLACK;
    }
    //printf("\n");
    return;

}

void Graph::dfs(int source){
    if(source < 0 || source >= nVertices) return;
    color_info = new int[nVertices];
    parent = new int[nVertices];
    for(int i = 0; i < nVertices; i++){
        color_info[i] = WHITE;
        parent[i] = -1;
    }
    q.enqueue(source);
    int qlength = 1;
    parent[source] = -1;
    int front = source;
    color_info[front] = GREY;
    printf("%d ",front);

    while(1){
        int flag = 0;
        for(int i = 0; i < nVertices; i++){
            if(isEdge(front,i) && color_info[i] == WHITE){
                flag = 1;
                color_info[i] = GREY;
                parent[i] = front;
                printf("%d ",i);
                q.enqueue(i);
                qlength++;
                front = i;
                for(int j = 1; j < qlength; j++)
                    q.enqueue(q.dequeue());
                break;
            }
        }
        if(!flag){
            int mode = 0;
            color_info[front] = BLACK;
            q.dequeue();
            qlength--;
            if(qlength == 0){
                for(int k = 0; k < nVertices; k++){
                    if(color_info[k] == WHITE){
                        front = k;
                        color_info[front] = GREY;
                        printf("%d ",front);
                        mode = 1;
                        break;
                    }
                }
                if(!mode)
                    break;
            }
            else{
                front = q.dequeue();
                qlength--;
            }
            q.enqueue(front);
            qlength++;
            for(int j = 1; j < qlength; j++)
                q.enqueue(q.dequeue());
        }

    }
    printf("\n");
}

int Graph::getDist(int u, int v)
{
    //returns the shortest path distance from u to v
    //must call bfs using u as the source vertex, then use distance array to find the distance
    if(u < 0 || u >= nVertices || v < 0 || v >= nVertices) return -1;
    bfs(u);
    return distance[v];
}


void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i);
        for(int j=0; j<nVertices;j++)
        {
            if(matrix[i][j]==1)
                printf(" %d", j);
        }
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(distance) delete[] distance;
    if(color_info) delete[] color_info;
    if(parent) delete[] parent;
    for(int i = 0; i < nVertices; i++)
        delete matrix[i];
    delete[] matrix;
    printf("destructor called\n");
}


//**********************Graph class ends here******************************


//******main function to test your code*************************
int main(void)
{
    int n;
    int choice;
    bool dir;

    LARGE_INTEGER frequency;
    LARGE_INTEGER t1,t2;
    double elapsedTime;


    printf("Enter your choice:\n");
    printf("1. directed graph   2. undirected graph\n");
    scanf("%d",&choice);
    if(choice == 1)dir = true;
    else if(choice == 2)dir = false;

    Graph g(dir);
    printf("Enter number of vertices: ");
    scanf("%d", &n);
    g.setnVertices(n);

    while(1)
    {
        printf("1. Add edge. \n");
        printf("2.Remove edge. 3.Is edge. 4.Get in degree. 5.Get out degree. 6.has common adjacent. 7.run BFS. 8.run DFS. 9.get distant. 10. Print Graph. 11.Statistics. 12. Exit.\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1) ///add edges
        {
            int u, v;
            scanf("%d%d", &u, &v);
            int added = g.addEdge(u, v);
        }
        else if(ch==2)  ///remove edges
        {
            int u,v;
            scanf("%d%d", &u, &v);
            g.removeEdge(u, v);
        }
        else if(ch==3) /// is edge
        {
            int u, v;
            scanf("%d%d",&u, &v);
            bool isedge= g.isEdge(u,v);
            if(isedge)
                printf("there is an edge between these two vertices\n");
            else
                printf("there is no edge between these two vertices\n");
        }
        else if(ch == 4)  /// get in degree
        {
            int u;
            printf("vertex: ");
            scanf("%d",&u);
            int inDegree = g.getInDegree(u);
            printf("In Degree: %d\n",inDegree);
        }
        else if(ch == 5)  /// get out degree
        {
            int u;
            printf("vertex: ");
            scanf("%d",&u);
            int outDegree = g.getOutDegree(u);
            printf("Out Degree: %d\n",outDegree);
        }
//        else if(ch == 5) /// print adjacent vertices
//        {
//            int u;
//            scanf("%d",&u);
//            g.printAdjVertices(u);
//        }
        else if(ch == 6)  /// has common adjacent
        {
            int u,v;
            scanf("%d%d",&u,&v);
            bool common = g.hasCommonAdjacent(u,v);
            if(common)
                printf("have common adjacents\n");
            else
                printf("No common adjacents\n");
        }
        else if(ch == 7) /// BFS
        {
            int source;
            scanf("%d",&source);
            g.bfs(source);
        }
        else if(ch == 8) /// DFS
        {
            int source;
            scanf("%d",&source);
            g.dfs(source);
        }
        else if(ch == 9) /// get distant
        {
            int u,v;
            scanf("%d%d",&u, &v);
            int distance = g.getDist(u,v);
            printf("Shortest distance: %d\n",distance);
        }
        else if(ch==10) ///print graph
        {
            g.printGraph();
        }
        else if(ch == 11) /// statistics
        {
            int edges=0,u,v,vertices,source,count = 0;
            srand(time(0));
            int loop = 0;
            while(loop < 6){
                printf("Vertices: ");
                scanf("%d",&vertices);
                g.setnVertices(vertices);
                edges = vertices;
                QueryPerformanceFrequency(&frequency);
                while(edges < (vertices*vertices - vertices)/8){
                    while(count < edges){
                        u = rand()%vertices;
                        v = rand()%vertices;
                        if(g.addEdge(u,v))
                            count++;
                    }

                    QueryPerformanceCounter(&t1);
                    for(int i = 0; i < 10; i++){
                        source = rand()%vertices;
                        g.bfs(source);
                    }
                    QueryPerformanceCounter(&t2);
                    elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000000.0 / frequency.QuadPart;
                    cout << "Required time of bfs for " << vertices << " vertices and " << edges << " edges: " << elapsedTime/10 << endl;
                    //cout << elapsedTime/10 << endl;
                    edges *= 2;
                }
                loop++;
            }


        }
        else if(ch==12) /// Exit
        {
            break;
        }
    }

    return 0;

}
