




#include <stdint.h>
#include <string>
#include <iostream>

#include <intrin.h> // Required library for instrinsics






using namespace std;



string get_cpu_name() {
	uint32_t data[4] = { 0 };

	_asm{
		cpuid;
		mov data[0], ebx;
		mov data[4], edx; // array index is number of bits not size_t of data[]
		mov data[8], ecx;

	}
	

	return string((const char*) data);

}


void assembler() {
	cout << "CPU is: " << get_cpu_name() << endl; // GenuineIntel, holy shit

	float f1[] = { 1.f, 2.f, 3.f, 4.f };
	float f2[] = { 5.f, 4.f, 3.f, 2.f };
	float result[4] = { 0.f };

	_asm {
		movups xmm1, f1;
		movups xmm2, f2;
		mulps xmm1, xmm2;
		movups result, xmm1;
	}

	for (auto i : result) {
		cout << i << "\t";
	}
	cout << endl;

	int d, c;
	_asm {
		mov eax, 1;
		cpuid;
		mov d, edx;
		mov c, ecx;
	}


	// https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions
	// https://en.wikipedia.org/wiki/CPUID
	// These bitwise ops are just looking up the bits to tell us which thing is/isn't supported
	if ((d & (1 << 26)) != 0) {
		cout << "SSE2 is supported" << endl;
	}

	if ((c & 1 ) != 0) {
		cout << "SSE3 is supported" << endl;
	}

	if ((c & (1 << 19)) != 0) {
		cout << "SSE4.1 is supported" << endl;
	}

	if ((c & (1 << 20)) != 0) {
		cout << "SSE4.2 is supported" << endl;
	}

}


void intrinsics() {
	int cpu_info[4];
	__cpuid(cpu_info, 1);
	cout << cpu_info << endl;

}




int main(int argc, char* argv[]) {
	
	//assembler();
	intrinsics();
	

}