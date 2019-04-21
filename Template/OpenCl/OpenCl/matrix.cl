__kernel void matrix(__global int *data) {

	int row = get_global_id(1);
	int colSize = get_global_size(0);
	int col = get_global_id(0);

    // Get the index of the current element to be processed
    int i = row * colSize + col;
 
    // Do the operation
	data[i] = data[i] * 2;
}