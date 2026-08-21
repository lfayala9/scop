#ifndef WINDOW_HPP
#define WINDOW_HPP
#define GLFW_INCLUDE_VULKAN
#define WIDTH 1080
#define HEIGHT 720
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
#include <vector>
#include <stdexcept>

const std::vector<const char *>	validation_layers = {
	"VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
constexpr bool enable_validation = false;
#else
constexpr bool enable_validation = true;
#endif

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
			void 	debug_message();
   			 static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																VkDebugUtilsMessageTypeFlagsEXT messageType,
																const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
																void* pUserData);
			vk::raii::Context	context;
			vk::raii::DebugUtilsMessengerEXT debug_msg = nullptr;
			vk::raii::Instance	instance = nullptr;
			uint32_t			width;
			uint32_t			height;
			GLFWwindow			*wdw;
};

#endif