#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#ifdef __APPLE__
#include <OpenCL/opencl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <iostream>
#include <fstream>
#include <array>

using namespace std;
using namespace cl;

// due to a bug I am forced to return the device alongside the program
pair<Program,Device> CreateProgram(const string &file) {
	vector<Platform> platforms;
	Platform::get(&platforms);

	auto platform = platforms.front();
	vector<Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	auto device = devices.front();
	ifstream programFile(file);
	string src(istreambuf_iterator<char>(programFile), (istreambuf_iterator<char>()));
	
	Program::Sources sources(1, make_pair(src.c_str(), src.length() + 1));

	Context context(device);

	Program program(context, sources);
	program.build("-cl-std=CL1.2");
	
	return make_pair(program,device);
}

int main(void) {

	pair<Program,Device> pair = CreateProgram("squareMatrixMultiplication.cl");

	Program program = pair.first;
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	//auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	//auto &device = devices.front();

	Device device = pair.second;
	//auto vendor = device.getInfo<CL_DEVICE_VENDOR>();

	
	const int cols = 2;
	const int rows = 2;
	const int size = cols * rows;

	array<array<int, cols>, rows> matrixA = { { {1,2},{3,4} } };
	array<array<int, cols>, rows> matrixB = { { {2,0},{1,2} } };
	array<array<int, cols>, rows> matrixC;


	Buffer bufA(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * size, matrixA.data());
	Buffer bufB(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * size, matrixB.data());
	Buffer bufC(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * size);

	Kernel kernel(program, "matrixMultiplication");
	kernel.setArg(0, bufA);
	kernel.setArg(1, bufB);
	kernel.setArg(2, bufC);


	CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(cols, rows));
	queue.enqueueReadBuffer(bufC,CL_TRUE,0,sizeof(int) * size, matrixC.data()) ;

	finish();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout<<" "<< matrixA[i][j] <<" ";
		}
		cout<<"  ";
		for (int j = 0; j < cols; j++) {
			cout<<" "<< matrixB[i][j] << " ";
		}
		cout<<"  ";
		for (int j = 0; j < cols; j++) {
			cout<< " "<< matrixC[i][j] <<" ";
		}
		cout << endl;
	}
}