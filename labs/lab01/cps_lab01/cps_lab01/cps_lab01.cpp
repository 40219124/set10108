// cps_lab01.cpp : Defines the entry point for the application.
//

#include "cps_lab01.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <functional>
#include <fstream>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

enum SUBSECTION
{
	HELLO, SLEEP, PARAMS, LAMBDA, LAMBTHREAD, DATA, MONTE
};

/* HELLO */
void hello_world() {
	cout << "Hello from thread " << this_thread::get_id() << endl;
}

/* SLEEP */
void task_one() {
	cout << "Task one starting" << endl;
	cout << "Task one sleeping for 3 seconds" << endl;
	sleep_for(seconds(3));
	cout << "Task one awake again" << endl;
	cout << "Task one sleeping for 5 seconds" << endl;
	sleep_for(milliseconds(5000));
	cout << "Task one awake again" << endl;
	cout << "Task one ending" << endl;
}

void task_two() {
	cout << "Task two starting" << endl;
	cout << "Task two sleeping for 2 seconds" << endl;
	sleep_for(microseconds(2000000));
	cout << "Task two awake again" << endl;
	cout << "Task two sleeping for 10 seconds" << endl;
	sleep_for(seconds(10));
	cout << "Task two awake again" << endl;
	cout << "Task two ending" << endl;
}

/* PARAMS */
constexpr size_t num_threads = 100;

void task(size_t n, int val) {
	cout << "Thread: " << n << " Random Value: " << val << endl;
}

/* DATA */
void work() {
	int n = 0;
	for (int i = 0; i < 1000000; ++i) {
		++n;
	}
}


/* MONTE */
void monte_carlo_pi(size_t iterations) {
	random_device r;
	default_random_engine e(r());
	uniform_real_distribution<double> distribution(0.0, 1.0);

	unsigned int in_circle = 0;
	for (size_t i = 0; i < iterations; ++i) {
		auto x = distribution(e);
		auto y = distribution(e);
		auto length = sqrt((x*x) + (y*y));
		if (length <= 1.0) {
			++in_circle;
		}
	}
	auto pi = (4.0 * in_circle) / static_cast<double>(iterations);
}

int main()
{
	SUBSECTION sub = MONTE;
	if (sub == HELLO) {
		thread t(hello_world);
		t.join();
	}
	else if (sub == SLEEP) {
		cout << "Starting task one" << endl;
		thread t1(task_one);
		cout << "Starting task two" << endl;
		thread t2(task_two);
		cout << "Joining task one" << endl;
		t1.join();
		cout << "Task one joined" << endl;
		cout << "Joining task two" << endl;
		t2.join();
		cout << "Task two joined" << endl;
		cout << "Exiting" << endl;
	}
	else if (sub == PARAMS) {
		random_device r;
		default_random_engine e(r());

		vector<thread> threads;
		for (size_t i = 0; i < num_threads; ++i) {
			threads.push_back(thread(task, i, e()));
		}

		for (auto &t : threads) {
			t.join();
		}
	}
	else if (sub == LAMBDA) {
		auto add = [](int x, int y) {return x + y; };
		auto x = add(10, 12);
		cout << "10 + 12 = " << x << endl;

		function<int(int, int)> add_function = [](int x, int y) {return x + y; };
		x = add_function(20, 12);
		cout << "20 + 12 = " << x << endl;

		int a = 5;
		int b = 10;
		auto add_fixed = [a, b] {return a + b; };
		x = add_fixed();
		cout << "5 + 10 = " << x << endl;

		a = 20;
		b = 30;
		x = add_fixed();
		cout << "20 + 30 = " << x << endl;

		auto add_reference = [&a, &b] {return a + b; };
		x = add_reference();
		cout << "20 + 30 = " << x << endl;

		a = 30;
		b = 5;
		x = add_reference();
		cout << "30 + 5 = " << x << endl;
	}
	else if (sub == LAMBTHREAD) {
		thread t([] {cout << "Hello from lambda thread!" << endl; });
		t.join();
	}
	else if (sub == DATA) {
		ofstream data("data.csv", ofstream::out);
		for (int i = 0; i < 100; ++i) {
			auto start = system_clock::now();
			thread t(work);
			t.join();
			auto end = system_clock::now();
			auto total = end - start;
			data << total.count() << endl;
		}
		data.close();
	}
	else if (sub == MONTE) {
		ofstream data("montecarlo.csv", ofstream::out);
		for (size_t num_threads = 0; num_threads <= 6; ++num_threads) {
			auto total_threads = static_cast<unsigned int>(pow(2.0, num_threads));
			cout << "Number of threads = " << total_threads << endl;
			data << "num_threads_" << total_threads;
			for (size_t iters = 0; iters < 100; ++iters) {
				auto start = system_clock::now();
				vector<thread> threads;
				for (size_t n = 0; n < total_threads; ++n) {
					threads.push_back(thread(monte_carlo_pi, static_cast<unsigned int>(pow(2.0, 24.0 - num_threads))));
				}
				for (auto &t : threads) {
					t.join();
				}
				auto end = system_clock::now();
				auto total = end - start;
				data << ", " << duration_cast<milliseconds>(total).count();
			}
			data << endl;
		}
		data.close();
	}

	system("pause");
	return 0;
}
