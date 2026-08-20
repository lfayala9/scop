#ifndef WINDOW_HPP
#define WINDOW_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

class window
{
	public:
			window(uint32_t w, uint32_t h);
			~window();
	private:
			void	init_window();
			void	init_vulkan();
			void	main_loop();
			uint32_t WIDTH;
			uint32_t HEIGHT;
			GLFWwindow	*wdw;
};

#endif