#include "window.hpp"

int	main(int ac, char **av)
{
	// if (ac != 2)
	// 	exit(EXIT_FAILURE);
	try
	{
		window w(800, 600);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return EXIT_SUCCESS;
}