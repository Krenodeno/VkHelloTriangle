#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "ComputeApp.hpp"

int main (int argc, char* argv[]) {

	ComputeApp app;

	try {
		app.run();
	}
	catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
