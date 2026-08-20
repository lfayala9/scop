#include "window.hpp"

window::window(uint32_t w, uint32_t h) : width(w), height(h)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	wdw = glfwCreateWindow(WIDTH, HEIGHT, "scop", nullptr, nullptr);
	if (!wdw)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize");
	}
	init_vulkan();
}

void	window::instance_vulkan()
{
	VkApplicationInfo	app_info{};
	app_info.pApplicationName = "Scop";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_3;
	
	uint32_t			extension_count = 0;
	auto				extensions = glfwGetRequiredInstanceExtensions(&extension_count);
	auto				extension_properties = context.enumerateInstanceExtensionProperties();
	for (uint32_t i = 0; i < extension_count; ++i)
	{
		if (std::none_of(extension_properties.begin(), extension_properties.end(), \
			[extensions = extensions[i]](auto const &ext_property) \
			{ return strcmp(ext_property.extensionName, extensions) == 0;}))
			throw std::runtime_error("Required GLFW extension not supported: " + std::string(extensions[i]));
	}
	VkInstanceCreateInfo	create_info{};
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = extension_count;
	create_info.ppEnabledExtensionNames = extensions;
	instance = vk::raii::Instance(context, create_info);
}

void	window::run()
{
	main_loop();
}

void	window::init_vulkan()
{
	instance_vulkan();
}

void	window::main_loop()
{
    while (!glfwWindowShouldClose(wdw))
        glfwPollEvents();
}

window::~window()
{
	if (wdw)
    	glfwDestroyWindow(wdw);
    glfwTerminate();	
}
