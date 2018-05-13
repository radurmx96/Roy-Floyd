#define INF 2

__global__ void roy_floyd (float* C, int N)
{
   int i = threadIdx.x;
   int j = threadIdx.y;
   int k = threadIdx.z;
   
   for (k=1;k<=N;k++)
      for (i=1;i<=N;i++)
         for (j=1;j<=N;j++)
            if (C[i][k]!=INF && C[k][j]!=INF)
               if (C[i][j]>C[i][k]+C[k][j])
               {
                  C[i][j] = C[i][k]+C[k][j];
               }
}
        
// Host code
int main()
{
    int N = 5;
    size_t size = N * sizeof(float);

    // Allocate input vector graph in host memory
    float* graph = (float*)malloc(size);

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
    float* d_graph;
    cudaMalloc(&d_graph, size);

    // Copy vectors from host memory to device memory
    cudaMemcpy(d_graph, graph, size, cudaMemcpyHostToDevice);

    // Invoke kernel
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks(N / threadsPerBlock.x, N / threadsPerBlock.y, N / threadsPerBlock.z);
    roy_floyd<<<blocksPerGrid, threadsPerBlock>>>(d_graph, N);

    // Copy result from device memory to host memory
    // h_C contains the result in host memory
    cudaMemcpy(d_graph, graph, size, cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_graph);
}
