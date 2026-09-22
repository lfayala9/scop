#ifndef APP_HPP
#define APP_HPP
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
#include <map>
#include <vector>
#include <stdexcept>

const std::vector<const char *>	validation_layers = {
	"VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
inline bool enable_validation = false;
#else
inline bool enable_validation = true;
#endif

class app
{
	public:
			app(uint32_t w, uint32_t h);
			~app();
			void	run();
	private:
			void									instance_vulkan();
			void									init_vulkan();
			void									main_loop();
			void									get_device();
			bool									is_suitable(vk::raii::PhysicalDevice const &device);
			void 									debug_message();
   			static VKAPI_ATTR VkBool32 VKAPI_CALL	debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
																VkDebugUtilsMessageTypeFlagsEXT messageType,
																const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
																void* pUserData);
			vk::raii::Context					context;
			vk::raii::Instance					instance = nullptr;
			vk::raii::DebugUtilsMessengerEXT 	debug_msg = nullptr;
			vk::raii::PhysicalDevice			device = nullptr;
			std::vector<const char*> 			requiredDeviceExtension = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			uint32_t							width;
			uint32_t							height;
			GLFWwindow							*wdw;
};

#endif
