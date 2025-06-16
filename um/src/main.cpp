#include <iostream>
#include "utils/driver.h"

int main()
{
	printf("[i] program started\n");

	// find process		
	globals::process_id = driver::get_process_id("cs2.exe");
	if (!globals::process_id) {
		printf("[e] unable to find process 'cs2.exe'\n");
		std::cin.get();
	}
	printf("[i] found process id: %p\n", globals::process_id);

	// find client
	globals::client = driver::get_client(globals::process_id);
	if (!globals::client) {
		printf("[e] unable to find client\n");
		std::cin.get();
	}
	printf("[i] found client: %p\n", (void*)globals::client);

	// find engine
	globals::engine = driver::get_engine(globals::process_id);
	if (!globals::engine) {
		printf("[e] unable to find engine\n");
		std::cin.get();
	}
	printf("[i] found engine: %p\n", (void*)globals::engine);

	std::cin.get();
	return 0;
}