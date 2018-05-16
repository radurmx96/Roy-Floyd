#define INF 2

__global__ void roy_floyd (int* C, int N)
{
   int i = threadIdx.x;
   int j = threadIdx.y;

   if (C[i][procid]!=INF && C[procid][j]!=INF)
     if (C[i][j]>C[i][procid]+C[procid][j])
     {
        C[i][j] = C[i][procid]+C[procid][j];
     }
}
        
// Host code
int main()
{
    int N = 5;
    size_t size = N * N * sizeof(int);

    // Allocate input vector graph in host memory
    int* graph = (int*)malloc(size);

    // Initialize input vectors
    int graph[N][N] =
	{
		0,2,5,4,2,
		1,0,0,3,2,
		6,6,0,4,5,
		7,9,2,0,7,
		6,9,3,2,0
	};

    // Allocate vectors in device memory
    int* d_graph;
    cudaMalloc(&d_graph, size);

    // Copy vectors from host memory to device memory
    cudaMemcpy(d_graph, graph, size, cudaMemcpyHostToDevice);

    // Invoke kernel
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks(N / threadsPerBlock.x, N / threadsPerBlock.y);
    roy_floyd<<<blocksPerGrid, threadsPerBlock>>>(d_graph, N);

    // Copy result from device memory to host memory
    // h_C contains the result in host memory
    cudaMemcpy(d_graph, graph, size, cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_graph);
}
