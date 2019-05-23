__kernel void matrixMultiplication(__global const int *A, __global const int *B, __global int *C) {

	int row = get_global_id(1);
	int rowSize = get_global_size(1);
	int colSize = get_global_size(0);
	int col = get_global_id(0);

    // Get the index of the current element to be processed
    int i = row * colSize + col;
 
	int sum = 0;
	for(int j = 0; j < rowSize; j++){
		sum += A[row * colSize + j] * B[j * colSize + col];
	}
	C[i] = sum;
}