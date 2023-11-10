// Topic : Jhonson’s Algorithem for all shortest paths
// Instructor : Anil shukla
// Mentor : E Harshith
// 1st Member : Sarthak Dev Rathor
// 2nd Member : Priyanshu 
// 3rd Member : Swapnil pandey
// Reqired Libraries
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 1000

// Structure to represent a neighbor node in the adjacency list

struct Neighbour
{
    // The destination vertex
    int destination;

    // The weight of the edge

    int weight;
    // Pointer to the next neighbor

    struct Neighbour *next;
};

// Structure to represent a

struct Graph
{
    // The number of vertices in the graph

    int vertices;

    // Adjacency list for each vertex

    struct Neighbour *adjacencyList[V];
};

// Function to add an edge to the graph

void addEdge(struct Graph *graph, int source, int destination, int weight)
{
    // Allocate memory for a new neighbor node

    struct Neighbour *newNode = (struct Neighbour *)malloc(sizeof(struct Neighbour));
    newNode->destination = destination;
    newNode->weight = weight;

    // Insert the new node at the beginning of the adjacency list for the source vertex

    newNode->next = graph->adjacencyList[source];
    graph->adjacencyList[source] = newNode;
}

// Function to find the vertex with the minimum distance that has not been visited

int findMinDistanceVertex(int distance[], int isVisited[], int v)
{
    // Initialize a variable to store the minimum distance

    int minDistance = INT_MAX;

    // Initialize a variable to store the index of the vertex with the minimum distance

    int minIndex = -1;

    // Iterate through all vertices

    for (int vertex = 0; vertex < v; vertex++)
    {
        // Check if the vertex has not been visited and if its distance is less than or equal to the current minimum

        if (!isVisited[vertex] && distance[vertex] <= minDistance)
        {
            // Update the minimum distance

            minDistance = distance[vertex];

            // Update the index of the vertex with the minimum distance

            minIndex = vertex;
        }
    }

    // Return the index of the vertex with the minimum distance

    return minIndex;
}

// Function to perform Dijkstra's algorithm to find the shortest path from a source vertex

void dijkstra(struct Graph *graph, int source, int *distance, int v)
{
    // Initialize the visited array

    int isVisited[V] = {0};

    for (int i = 0; i < v; i++)
    {
        // Initialize distances to all vertices as infinity

        distance[i] = INT_MAX;
    }
    // Distance from the source vertex to itself is 0

    distance[source] = 0;

    for (int i = 0; i < v; i++)
    {
        // Find the vertex with the minimum distance

        int minDistanceVertex = findMinDistanceVertex(distance, isVisited, v);

        // Mark the vertex as visited

        isVisited[minDistanceVertex] = 1;

        // Initialize a pointer to the first neighbor of the current vertex

        struct Neighbour *neighbor = graph->adjacencyList[minDistanceVertex];

        // Iterate through the neighbor vertices

        while (neighbor != NULL)
        {
            // Get the destination vertex and the weight of the edge

            int destination = neighbor->destination;
            int weight = neighbor->weight;

            // Check if a shorter path to the destination is found

            if (!isVisited[destination] && distance[minDistanceVertex] != INT_MAX &&
                distance[minDistanceVertex] + weight < distance[destination])
            {
                // Update the distance to the destination if a shorter path is available

                distance[destination] = distance[minDistanceVertex] + weight;
            }

            // Move to the next neighbor

            neighbor = neighbor->next;
        }
    }
}

// Function to perform the Bellman-Ford algorithm to find the shortest paths from a source vertex

int bellmanFord(struct Graph *graph, int source, int *distance, int v)
{
    // Initialize distances to all vertices as infinity

    for (int i = 0; i < v; i++)
    {
        distance[i] = INT_MAX;
    }

    // Distance from the source vertex to itself is 0

    distance[source] = 0;

    // Iterate for (v-1) iterations to relax edges and find shortest paths

    for (int i = 0; i < v - 1; i++)
    {
        // Iterate through all vertices

        for (int currentVertex = 0; currentVertex < v; currentVertex++)
        {
            // Get the first neighbor of the current vertex

            struct Neighbour *neighbor = graph->adjacencyList[currentVertex];

            // Iterate through neighbors of the current vertex

            while (neighbor != NULL)
            {
                // Check if a shorter path to the neighbor is found

                if (distance[currentVertex] != INT_MAX && distance[currentVertex] + neighbor->weight < distance[neighbor->destination])
                {
                    // Update the distance to the neighbor

                    distance[neighbor->destination] = distance[currentVertex] + neighbor->weight;
                }
                neighbor = neighbor->next;
            }
        }
    }

    // Check for negative weight cycles

    for (int currentVertex = 0; currentVertex < v; currentVertex++)
    {
        // Get the first neighbor of the current vertex

        struct Neighbour *neighbor = graph->adjacencyList[currentVertex];

        // Iterate through neighbors of the current vertex

        while (neighbor != NULL)
        {
            // Check for negative weight cycle

            if (distance[currentVertex] != INT_MAX && distance[currentVertex] + neighbor->weight < distance[neighbor->destination])
            {
                // Negative weight cycle detected

                return 0;
            }
            neighbor = neighbor->next;
        }
    }

    // No negative weight cycle detected, return 1

    return 1;
}

// Function to perform Johnson's algorithm to find all-pairs shortest paths

int **johnsons(struct Graph *graph, int v)
{
    // Add a new vertex to the original graph

    v++;
    graph->adjacencyList[v - 1] = (struct Neighbour *)malloc(v * sizeof(struct Neighbour));

    for (int i = 0; i < v - 1; i++)
    {
        // Initialize the new vertex with zero weight to other vertices

        graph->adjacencyList[v - 1][i].destination = i;
        graph->adjacencyList[v - 1][i].weight = 0;
    }

    // Calculate the potential (h values) using the Bellman-Ford algorithm

    int *h = (int *)malloc(v * sizeof(int));
    int state = bellmanFord(graph, v - 1, h, v + 1);

    // Check for negative weight cycles detected by Bellman-Ford

    if (!state)
    {
        // Negative weight cycle detected

        return NULL;
    }

    // Re-weight the edges of the original graph based on the potential (h values)
    // Loop through each vertex in the graph (except the added vertex)

    for (int u = 0; u < v - 1; u++)
    {
        // Get the list of neighbors for the current vertex

        struct Neighbour *neighbor = graph->adjacencyList[u];

        // Iterate through the neighbor vertices

        while (neighbor != NULL)
        {
            // Get the destination vertex of the neighbor

            int v = neighbor->destination;

            // Get the weight of the edge to the neighbor vertex

            int w = neighbor->weight;

            // Update the weight of the edge based on potential (h values)

            neighbor->weight = w + h[u] - h[v];

            // Move to the next neighbor in the list

            neighbor = neighbor->next;
        }
    }

    // Remove the edge connecting to the new vertex

    free(graph->adjacencyList[v - 1]);
    v--;

    // Initialize a 2D array for distances

    int **distances = (int **)malloc(v * sizeof(int *));
    for (int i = 0; i < v; i++)
    {
        distances[i] = (int *)malloc(v * sizeof(int));

        // Calculate the shortest paths using Dijkstra's algorithm for each vertex

        dijkstra(graph, i, distances[i], v);
    }

    // Adjust distances using the potential (h values)
    // Loop through all vertices 'u' in the graph

    for (int u = 0; u < v; u++)
    {
        // Loop through all vertices 'v1' in the graph

        for (int v1 = 0; v1 < v; v1++)
        {
            // Check if there is a valid distance between 'u' and 'v1'

            if (distances[u][v1] != INT_MAX)
            {
                // Adjust the distance by the potential difference between 'v1' and 'u'

                distances[u][v1] += h[v1] - h[u];
            }
        }
    }

    // Return the calculated distances

    return distances;
}

int main()
{
    // Initialize the graph

    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));

    // Prompt the user to enter the number of vertices

    printf("Please Enter the Number of vertices in the graph: ");
    int v;
    scanf("%d", &v);
    graph->vertices = v;

    // Initialize adjacency list for each vertex

    for (int i = 0; i < v; i++)
    {
        graph->adjacencyList[i] = NULL;
    }

    // Prompt the user to enter the graph matrix

    int matrix[v][v];
    printf("Please Enter the graph Matrix:\n");
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }

    // Convert the adjacency matrix to the graph by adding edges

    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            if (matrix[i][j] != 0)
            {
                addEdge(graph, i, j, matrix[i][j]);
            }
        }
    }

    // Call Johnson's algorithm to find all-pairs shortest paths

    int **distances = johnsons(graph, v);

    if (distances == NULL)
    {
        printf("Negative weight cycle detected.\n");
        return 0;
    }

    // Display the distance matrix

    printf("Distance matrix:\n");
    printf("\t");
    for (int i = 0; i < v; i++)
    {
        printf("%d\t", i);
    }

    for (int i = 0; i < v; i++)
    {
        printf("\n%d\t", i);
        for (int j = 0; j < v; j++)
        {
            if (distances[i][j] == INT_MAX)
            {
                // Using X to represnt infinity

                printf("X\t");
            }
            else
            {
                printf("%d\t", distances[i][j]);
            }
        }
    }

    return 0;
}
