#include "App.hpp"

int main(int argc, char** argv)
{
	App myApp{};
	try
	{
		myApp.initialize();
		myApp.run();
		myApp.cleanup();
	}
	catch (std::runtime_error& excpt)
	{
		std::cerr << excpt.what() << std::endl;
		std::cin.get();
	}
}

