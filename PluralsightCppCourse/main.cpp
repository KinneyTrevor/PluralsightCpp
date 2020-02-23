




#include <stdint.h>
#include <string>
#include <iostream>
#include <random>
#include <chrono>

#include <intrin.h> // Required library for instrinsics
#include "main.h"






using namespace std;
using namespace chrono;




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
	//int cpu_info[4];
	//__cpuid(cpu_info, 1);

	auto a = _mm_set_ps(1, 2, 3, 4);
	auto b = _mm_set_ps(4, 3, 2, 1);
	auto c = _mm_add_ps(a, b);

	float f = c.m128_f32[0];
	cout << f << endl;

	__try {
		__m256d z = _mm256_set_pd(1, 2, 3, 4);
	}
	__except (1) {
		cout << "You can't do this pall";
	}
}


void simple_mad(float* a, float* b, float* c, float* result, const int length) {
	for (size_t i = 0; i < length; i++) {
		result[i] = a[i] * b[i] + c[i];
	}
}



void optimization() {

	auto begin = high_resolution_clock::now();
	// Create some big boi arrays
	const int length = 1024 * 1024 * 64;
	float *a = new float[length];
	float *b = new float[length];
	float *c = new float[length];
	float *result = new float[length];


	mt19937_64 rng(random_device{}()); // Init rng generator
	uniform_real_distribution<float> dist(0, 1);

	for (size_t i = 0; i < length; i++) {
		a[i] = dist(rng);
		b[i] = dist(rng);
		c[i] = dist(rng);
	}
	auto end = high_resolution_clock::now();

	cout << "Array initialization took: " << duration_cast<milliseconds>(end - begin).count() << "milliseconds" << endl;
	
	begin = high_resolution_clock::now();
	simple_mad(a, b, c, result, length);
	end = high_resolution_clock::now();
	cout << "Multiply and add (MAD) opp took: " << duration_cast<milliseconds>(end - begin).count() << "milliseconds" << endl;
	// Takes 174 milliseconds with optimization turned off in project settings
	// A lot of the time is for rng to generate that many random numbers
	// Took 170 ms with optimizations turned on lol. Got 164
	// Actually wasn't building it properly, got it down to 88 miilliseconds!!
	// In the course he's using the intel compiler and got a 50% increase in speed

	// Settings I changed -
	// C/C++ settings, optimization, optimization as well as favor faster code
	// Also turned assembly generation on and set it to assembly w/ source code /FAs


	delete[] a;
	delete[] b;
	delete[] c;
	delete[] result;



}


int main(){


	
	//assembler();
	//intrinsics();
	optimization();
	

}