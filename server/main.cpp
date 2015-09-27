#include <iostream>
#include <string>
#include <asio.hpp>
#include "server/server.hpp"

using asio::ip::tcp;

int main() {
	try {
		tcp::endpoint endpoint(asio::ip::tcp::v4(), tec::networking::SERVER_PORT);
		tec::networking::Server server(endpoint);

		std::cout << "Server ready" << std::endl;
		server.Start();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}
