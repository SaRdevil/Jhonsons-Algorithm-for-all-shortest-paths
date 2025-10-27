// Topic : Johnson’s Algorithm for All-Pairs Shortest Paths
// Instructor : Anil Shukla
// Mentor : E Harshith
// 1st Member : Sarthak Dev Rathor
// 2nd Member : Priyanshu
// 3rd Member : Swapnil Pandey

#include <bits/stdc++.h>
using namespace std;

#define INF INT_MAX

// Structure to represent a neighbor node in the adjacency list
struct Neighbour {
    int destination;   // destination vertex
    int weight;        // weight of the edge
    Neighbour* next;   // pointer to next neighbor

    Neighbour(int dest, int w, Neighbour* nxt = nullptr)
        : destination(dest), weight(w), next(nxt) {}
};

// Structure to represent a graph
struct Graph {
    int vertices;                                  // number of vertices
    vector<Neighbour*> adjacencyList;              // adjacency list

    Graph(int v) : vertices(v), adjacencyList(v, nullptr) {}
};

// Function to add an edge to the graph
void addEdge(Graph* graph, int source, int destination, int weight) {
    Neighbour* newNode = new Neighbour(destination, weight, graph->adjacencyList[source]);
    graph->adjacencyList[source] = newNode;
}

// Function to find the vertex with the minimum distance not yet visited
int findMinDistanceVertex(vector<int>& distance, vector<int>& isVisited, int v) {
    int minDistance = INF;
    int minIndex = -1;

    for (int vertex = 0; vertex < v; vertex++) {
        if (!isVisited[vertex] && distance[vertex] <= minDistance) {
            minDistance = distance[vertex];
            minIndex = vertex;
        }
    }
    return minIndex;
}

// Dijkstra's algorithm
void dijkstra(Graph* graph, int source, vector<int>& distance, int v) {
    vector<int> isVisited(v, 0);
    distance.assign(v, INF);
    distance[source] = 0;

    cout << "Path " << source << " vertex to all vertex : \n";
    cout << source << " ";

    for (int i = 0; i < v; i++) {
        int minDistanceVertex = findMinDistanceVertex(distance, isVisited, v);
        if (minDistanceVertex == -1) break;

        if (distance[minDistanceVertex] < INF)
            cout << "-> " << minDistanceVertex;
        isVisited[minDistanceVertex] = 1;

        for (Neighbour* neighbor = graph->adjacencyList[minDistanceVertex];
             neighbor != nullptr; neighbor = neighbor->next) {
            int dest = neighbor->destination;
            int weight = neighbor->weight;

            if (!isVisited[dest] && distance[minDistanceVertex] != INF &&
                distance[minDistanceVertex] + weight < distance[dest]) {
                distance[dest] = distance[minDistanceVertex] + weight;
            }
        }
    }
    cout << "\n";
}

// Bellman-Ford algorithm
bool bellmanFord(Graph* graph, int source, vector<int>& distance, int v) {
    distance.assign(v, INF);
    distance[source] = 0;

    for (int i = 0; i < v - 1; i++) {
        for (int current = 0; current < v; current++) {
            for (Neighbour* neighbor = graph->adjacencyList[current];
                 neighbor != nullptr; neighbor = neighbor->next) {
                if (distance[current] != INF &&
                    distance[current] + neighbor->weight < distance[neighbor->destination]) {
                    distance[neighbor->destination] = distance[current] + neighbor->weight;
                }
            }
        }
    }

    // check for negative cycles
    for (int current = 0; current < v; current++) {
        for (Neighbour* neighbor = graph->adjacencyList[current];
             neighbor != nullptr; neighbor = neighbor->next) {
            if (distance[current] != INF &&
                distance[current] + neighbor->weight < distance[neighbor->destination]) {
                return false; // negative cycle detected
            }
        }
    }
    return true;
}

// Johnson's algorithm
vector<vector<int>> johnsons(Graph* graph, int v) {
    // Add a new vertex
    int newV = v + 1;
    graph->adjacencyList.push_back(nullptr);

    // Add zero-weight edges from new vertex to all others
    for (int i = 0; i < v; i++)
        addEdge(graph, newV - 1, i, 0);

    // Bellman-Ford to get potential (h values)
    vector<int> h(newV, INF);
    if (!bellmanFord(graph, newV - 1, h, newV)) {
        cout << "Negative weight cycle detected.\n";
        return {};
    }

    // Reweight edges
    for (int u = 0; u < v; u++) {
        for (Neighbour* neighbor = graph->adjacencyList[u];
             neighbor != nullptr; neighbor = neighbor->next) {
            int dest = neighbor->destination;
            neighbor->weight = neighbor->weight + h[u] - h[dest];
        }
    }

    // Remove added vertex
    graph->adjacencyList.pop_back();

    // Run Dijkstra for all vertices
    vector<vector<int>> distances(v, vector<int>(v, INF));
    for (int i = 0; i < v; i++) {
        dijkstra(graph, i, distances[i], v);
    }

    // Adjust distances back
    for (int u = 0; u < v; u++) {
        for (int v1 = 0; v1 < v; v1++) {
            if (distances[u][v1] != INF)
                distances[u][v1] += h[v1] - h[u];
        }
    }

    return distances;
}

// Main function
int main() {
    cout << "Please Enter the Number of vertices in the graph: ";
    int v;
    cin >> v;

    Graph* graph = new Graph(v);

    cout << "Please Enter the graph Matrix:\n";
    vector<vector<int>> matrix(v, vector<int>(v));
    for (int i = 0; i < v; i++)
        for (int j = 0; j < v; j++)
            cin >> matrix[i][j];

    // Convert matrix to adjacency list
    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            if (matrix[i][j] != 0)
                addEdge(graph, i, j, matrix[i][j]);
        }
    }

    // Run Johnson's algorithm
    vector<vector<int>> distances = johnsons(graph, v);
    if (distances.empty()) return 0;

    // Print distance matrix
    cout << "\nDistance matrix:\n\t";
    for (int i = 0; i < v; i++) cout << i << "\t";
    cout << "\n";

    for (int i = 0; i < v; i++) {
        cout << i << "\t";
        for (int j = 0; j < v; j++) {
            if (distances[i][j] == INF)
                cout << "X\t";
            else
                cout << distances[i][j] << "\t";
        }
        cout << "\n";
    }

    return 0;
}
