
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
