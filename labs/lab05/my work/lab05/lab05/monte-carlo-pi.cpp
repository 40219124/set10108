#include <iostream>
#include <random>
#include <mpi.h>

using namespace std;

double monte_carlo_pi(unsigned int iterations) {
	random_device r;
	default_random_engine e(r());
	uniform_real_distribution<double> distribution(0.0, 1.0);

	unsigned int in_circle = 0;
	for (unsigned int i = 0; i < iterations; ++i) {
		auto x = distribution(e);
		auto y = distribution(e);
		auto length = sqrt((x*x) + (y*y));
		if (length <= 1.0) {
			++in_circle;
		}
	}
	return (4.0*in_circle) / static_cast<double>(iterations);
}

int main(int argc, char **argv) {
	int num_procs, my_rank;
	
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		cout << "ERROR -initialising MPI" << endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	double local_sum, global_sum;

	local_sum = monte_carlo_pi(static_cast<unsigned int>(pow(2, 24)));
	cout.precision(numeric_limits<double>::digits10);
	cout << my_rank << ":" << local_sum << endl;
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (my_rank == 0) {
		global_sum /= (float)num_procs;
		cout << "Pi=" << global_sum << endl;
	}

	MPI_Finalize();

	return 0;
}