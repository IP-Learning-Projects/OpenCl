#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#ifdef __APPLE__
#include <OpenCL/opencl.hpp>
#else
#include <CL/cl.hpp>
#endif
#include <iostream>
#include <fstream>

using namespace std;
using namespace cl;

//65-90 A-Z
//97-122 a-z

int string_length(char s[]) {
	int c = 0;
	while (s[c] != '\0') {
		c++;
	}
	return c;
}

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

	pair<Program,Device> pair = CreateProgram("vigenere.cl");

	Program program = pair.first;
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	//auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	//auto &device = devices.front();

	Device device = pair.second;
	//auto vendor = device.getInfo<CL_DEVICE_VENDOR>();

	
	int len, lenKey;
	char inputString[200], key[200], outputString[200], mode;

	cout << "Choose mode : (E)ncrypt or (D)ecrypt"<<endl;
	cin >> mode;
	cout << "Input text to encrypt/decrypt" << endl;
	cin >>  inputString;
	cout << "Input key" << endl;
	cin >> key;


	len = string_length(inputString);
	lenKey = string_length(key);

	Buffer bufMode(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(char), &mode);
	Buffer bufInputString(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(char) * len, &inputString);
	Buffer bufKey(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(char) * lenKey, &key);
	Buffer bufLen(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int), &len);
	Buffer bufLenKey(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int), &lenKey);
	Buffer bufOutputString(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(char) * len);

	Kernel kernel(program, "vigenere");
	kernel.setArg(0, bufMode);
	kernel.setArg(1, bufInputString);
	kernel.setArg(2, bufLen);
	kernel.setArg(3, bufKey);
	kernel.setArg(4, bufLenKey);
	kernel.setArg(5, bufOutputString);

	CommandQueue queue(context, device);
	queue.enqueueNDRangeKernel(kernel,NullRange,NDRange(len));
	queue.enqueueReadBuffer(bufOutputString,CL_TRUE,0, sizeof(char) * len, &outputString);

	finish();

	outputString[len] = '\0';

	cout << "Input string:" << inputString << endl;
	cout << "key string:" << key << endl;
	cout << "Output string:" << outputString<< endl;

}