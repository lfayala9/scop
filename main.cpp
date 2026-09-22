#include "app.hpp"

int	main(int ac, char **av)
{
	// if (ac != 2)
	// 	exit(EXIT_FAILURE);
	(void)ac;
	(void)av;
	try
	{
		app win(WIDTH, HEIGHT);
		win.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return EXIT_SUCCESS;
}