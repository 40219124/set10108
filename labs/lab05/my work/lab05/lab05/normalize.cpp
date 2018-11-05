#include <iostream>
#include <mpi.h>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

void generate_data(vector<float> &data) {
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(static_cast<unsigned int>(millis.count()));
	for (unsigned int i = 0; i < data.size(); ++i) {
		data[i] = e();
	}
}

void normalise_vector(vector<float> &data) {
	for (unsigned int i = 0; i < data.size() / 4; ++i) {
		float sum = 0.0f;
		for (unsigned int j = 0; j < 4; ++j) {
			sum += powf(data[(i * 4) + j], 2.0f);
		}
		sum = sqrtf(sum);
		for (unsigned int j = 0; j < 4; ++j) {
			data[(i * 4) + j] /= sum;
		}
	}
}

int main(int argc, char **argv) {







	return 0;
}