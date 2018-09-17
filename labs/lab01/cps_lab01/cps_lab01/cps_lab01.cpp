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
	HELLO, SLEEP, PARAMS, LAMBDA, LAMBTHREAD, DATA
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


int main()
{
	SUBSECTION sub = DATA;
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

	system("pause");
	return 0;
}
