#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
#include <utility>
#include <algorithm> 
#include <math.h>
using std::pair;
using std::make_pair;
using std::queue;
using std::vector;
using namespace std;
/* This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly. */
class FlowGraph {
public:
    struct Edge {
        int from, to, capacity, flow;
    };

private:
    /* List of all - forward and backward - edges */
    vector<Edge> edges;

    /* These adjacency lists store only indices of edges in the edges list */
    vector<vector<size_t> > graph;

public:
    explicit FlowGraph(size_t n): graph(n) {}

    void add_edge(int from, int to, int capacity) {
        /* Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges */
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge); // irse en impares G
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);// Pares en G'
    }


    size_t size() const {
        return graph.size();
    }

    size_t sizeEdge()const{
        return edges.size();
    }

    const vector<size_t>& get_ids(int from) const {
        return graph[from];
    }

     Edge& get_edge(size_t id) {//quito los const, para que pueda modificar y ser modificado
        return edges[id];
    }

    void add_flow(size_t id, int flow) {
        /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
        edges[id].capacity -=flow;
        edges[id ^ 1].capacity +=flow;

        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }

       void update_flow(size_t id, int flow) {
        /* To get a backward edge for a true forward edge (i.e id is even), we should get id + 1
         * due to the described above scheme. On the other hand, when we have to get a "backward"
         * edge for a backward edge (i.e. get a forward edge for backward - id is odd), id - 1
         * should be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through! */
        edges[id].capacity += flow;
        edges[id ^ 1].flow -= flow;
    }


     void BFS(vector< pair<int,int>> &path, int s, int t){
       
       size_t  sizeGraph = this->size();
       vector<int> dist(sizeGraph,INT_MAX);
        vector< pair<int,int>> prev(sizeGraph);
        vector<bool> visited(sizeGraph, false);
        //vector< Edge> path;
        queue<size_t> Q;
        dist[s] = 0;
        Q.push(s);
      
        while( !Q.empty() )
        {
            int u = Q.front();
            Q.pop();
            if(!visited[u])
             {
                for ( int i =0 ; i < this->graph[u].size(); i++)//hacerlo menor quizas con get_ids.size()
                { 
                    int edges_index = this->graph[u][i]; //modificado
                     if( edges[edges_index].capacity != 0 && dist[this->edges[edges_index].to] == INT_MAX)// lo elimine dist[this->edges[edges_index].to] == INT_MAX
                    {
                            Q.push(edges[edges_index].to);
                            dist[this->edges[edges_index].to] = dist[u] +1;
                            prev[edges[edges_index].to] =  make_pair(u,edges_index);

                    }

                }

               visited[u] = true;

            }

        }
            if(!prev.empty())
               construcPath(prev,path,0,t);
        //return visited[t];
    }            

    //vector< Edge> construcPath(vector< pair<int,int>> &prev, int s, int u ){
    void construcPath(vector< pair<int,int>> &prev, vector< pair<int,int>> &path,int s, int u ){
        //vector< Edge> pathWithEdges;
        ///vector< pair<int,int>> pathWithEdges;
        while(u != s){
            path.push_back(make_pair(prev[u].first,prev[u].second));
            u = prev[u].first;
        }
        reverse(path.begin(),path.end());
        //return pathWithEdges;
    }
    //* Hacer el path*/
};


template <class T> const T& minimo (const T& a, const T& b) {
  return !(b.capacity<a.capacity)?a:b;     // or: return !comp(b,a)?a:b; for version (2)
}



FlowGraph read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    FlowGraph graph(vertex_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
    }
    return graph;
}

int max_flow(FlowGraph& Graph, int from, int to) {
    int flow = 0;
    bool CONTINUE = true;
    int num = 0;

    bool flag = true;
    do{
        vector< pair<int,int>> path;
        Graph.BFS(path,0,Graph.size()-1);

        if(Graph.sizeEdge()/2 == 1)
            flag = false;

        if(path.size()==1  && path[0].first == 0 && path[0].second == 0 && flag)
        {
            return flow;
        }

       // if(Graph.get_edge(0).from == Graph.get_edge(0).to)
       //     break;
        int min_index = 0;
        for (int i = 1; i < path.size(); ++i)
        {
           
            if(Graph.get_edge(path[i].second).capacity < Graph.get_edge(path[min_index].second).capacity)
                min_index = i;    //estaba con second   
        }

        int X = Graph.get_edge(path[min_index].second).capacity;
        //cout<<"El minimo flow es: "<<X<<endl;
        for (int i = 0; i < path.size(); ++i)
        {
            Graph.add_flow(path[i].second,X);//  es igual Graph.add_flow(path[i].second,X)
        }
        flow = flow+X;

}while(flag);

return flow;
}
int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();
    cout << max_flow(graph, 0, graph.size() - 1) << "\n";
    return 0;
}

/*
b 190 154 189 70
b 93
b 128
b 133

4 5         
1 2 10000
1 3 10000
2 3 1
3 4 10000
2 4 10000

5 7
1 2 2
2 5 5
1 3 6 
3 4 2
4 5 1
3 2 3
2 4 1

2 5 
1 1 10000 
1 2 1 
1 2 4 
1 2 100 
2 1 900

2 1 
1 2 10000


6 7
1 2 1
1 3 1
2 4 1
2 5 1
3 4 1
4 6 1
5 6 1


*/