// Topic: Floyd-Warshall
// Instructor: Anil Shukla
// Mentor: E Harshith
// 1st Member: Sarthak Dev Rathor
// 2nd Member: Priyanshu
// 3rd Member: Swapnil Pandey

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 1000
#define INF 10000000// Define Infinite as a large enough value

// A function to print the solution matrix
void printSolution(int dist[][V], int v);

// Solves the all-pairs shortest path problem using Floyd-Warshall algorithm
void floydWarshall(int dist[][V], int v)
{
    int i, j, k;

    // Add all vertices one by one to the set of intermediate vertices.
    // Before the start of an iteration, we have shortest distances between all pairs
    // of vertices such that the shortest distances consider only the vertices in set {0, 1, 2, .. k-1} as intermediate vertices.
    // After the end of an iteration, vertex no. k is added to the set of intermediate vertices and the set becomes {0, 1, 2, .. k}
    for (k = 0; k < v; k++)
    {
        // Pick all vertices as source one by one
        for (i = 0; i < v; i++)
        {
            // Pick all vertices as the destination for the above-picked source
            for (j = 0; j < v; j++)
            {
                // If vertex k is on the shortest path from i to j, then update the value of dist[i][j]
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    // Print the shortest distance matrix
    printSolution(dist, v);
}

/* A utility function to print the solution */
void printSolution(int dist[][V], int v)
{
    printf("The following matrix shows the shortest distances between every pair of vertices:\n");
    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            if (dist[i][j] == INF)
                printf("%s","X");
            else
                printf("%d", dist[i][j]);
        }
        printf("\n");
    }
}

// Driver's code
int main()
{
    printf("Please Enter the Number of vertices in the graph: ");
    int v ;
    scanf("%d", &v);
    int graph[V][V] ;
    printf("Enter the grapg : \n");
    for(int i=0;i<v;i++){
        for(int j=0;j<v;j++){
            scanf("%d",&graph[i][j]);
            if(graph[i][j]==0&&j!=i)
            graph[i][j]= INF;
        }
    }
    // Function call
    floydWarshall(graph, v);
    return 0;
}
