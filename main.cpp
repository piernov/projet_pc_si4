#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm>

#include "ConcurrentDeque.h"
#include "Map.h"
#include "Scenario0.h"
#include "Scenario1.h"

struct Args {
	int person_count = 0;
	int scenario = 0;
	bool benchmark_mode = false;
};

Args parse_args(int argc, char *argv[]) {
	Args args;

	// Command line parameters parsing
	for (int i = 0; i < argc; i++) {
		auto arg = argv[i];
		if (arg[0] != '-') continue;
		switch (arg[1]) {
			case 'p':
				args.person_count = arg[2] - '0';
				break;
			case 't':
				args.scenario = arg[2] - '0';
				break;
			case 'm':
				args.benchmark_mode = true;
				break;
			default:
				continue;
		}
	}

	args.person_count = 1 << args.person_count;

	std::cout << "Init: benchmark_mode = " << args.benchmark_mode << ", threads_mode = " << args.scenario << ", person_count = " << args.person_count << std::endl;

	return args;
}

int main(int argc, char* argv[]) {
	auto args = parse_args(argc, argv);
	std::vector<double> cputimes;
	std::vector<double> walltimes;
	Map map;


	// Map & initialization
	map.initV2(args.person_count);
	if (!args.benchmark_mode)
		map.print();
	

	// Run scenario
	
	for (auto i = 0; i < 1; i++) {
		Map tmpMap = map;
		std::unique_ptr<Scenario> scenario;
	
		switch (args.scenario) {
			default:
#if 0
			case 0:
				scenario = std::unique_ptr<Scenario>(new Scenario0(tmpMap));
				break;
#endif
			case 1:
				scenario = std::unique_ptr<Scenario>(new Scenario1(tmpMap));
				break;
		}

		scenario->setBenchmarkMode(args.benchmark_mode);
		scenario->run();
		walltimes.push_back(scenario->getCPUTime());
		cputimes.push_back(scenario->getWallTime());
	}

	if (args.benchmark_mode) {
		std::sort(walltimes.begin(), walltimes.end());
		std::sort(cputimes.begin(), cputimes.end());

		auto walltimemean = (walltimes[1] + walltimes[2] + walltimes[3]) / 3;
		auto cputimemean = (cputimes[1] + cputimes[2] + cputimes[3]) / 3;

		std::cout << std::fixed << std::setprecision(2) << "CPU time used: " << cputimemean << " ms" << std::endl;
		std::cout << "Wall clock time passed: " << walltimemean << " ms" << std::endl;
	}

	return 0;
}
