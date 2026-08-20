#ifndef WINDOW_HPP
#define WINDOW_HPP
#define GLFW_INCLUDE_VULKAN
#define WIDTH 1920
#define HEIGHT 1080
#include <GLFW/glfw3.h>
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#	include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <vulkan/vulkan.h>
#include <algorithm>
#include <ranges>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

class window
{
	public:
			window(uint32_t w, uint32_t h);
			~window();
			void	run();
	private:
			void	instance_vulkan();
			void	init_vulkan();
			void	main_loop();
			vk::raii::Context	context;
			vk::raii::Instance	instance = nullptr;
			uint32_t	width;
			uint32_t	height;
			GLFWwindow			*wdw;
};

#endif