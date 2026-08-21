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

std::vector<const char*> get_instance_extension()
{
    uint32_t glfwExtensionCount = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

void	window::debug_message()
{
	if (!enable_validation)
		return;
	vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
	                                                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
	vk::DebugUtilsMessageTypeFlagsEXT     messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{};
	debugUtilsMessengerCreateInfoEXT.messageSeverity = severityFlags;
	debugUtilsMessengerCreateInfoEXT.messageType     = messageTypeFlags;
	debugUtilsMessengerCreateInfoEXT.pfnUserCallback = &debugCallback;
	debug_msg = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
}

void	window::instance_vulkan()
{
	VkApplicationInfo	app_info{};
	app_info.pApplicationName = "Scop";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_3;
    std::vector<char const*> required_layers;

    if (enable_validation)
        required_layers.assign(validation_layers.begin(), validation_layers.end());
    auto layer_properties = context.enumerateInstanceLayerProperties();
    auto unsupportedLayerIt = std::ranges::find_if(required_layers,
		                                               [&layer_properties](auto const &required_layer) {
			                                               return std::ranges::none_of(layer_properties,
                                                                [required_layer](auto const &layer_property) { return strcmp(layer_property.layerName, required_layer) == 0; });
		                                               });
    if (unsupportedLayerIt != required_layers.end())
        throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
    auto required_extension = get_instance_extension();
    auto extension_properties = context.enumerateInstanceExtensionProperties();
    auto unsupported_properties =
        std::ranges::find_if(required_extension,
                                [&extension_properties](auto const &requried_extension) {
                                    return std::ranges::none_of(extension_properties,
                                                                [requried_extension](auto const &extension_property) { return strcmp(extension_property.extensionName, requried_extension) == 0; });
                                });
    if (unsupported_properties != required_extension.end())
		throw std::runtime_error("Required extension not supported: " + std::string(*unsupported_properties));
	VkInstanceCreateInfo	create_info{};
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = static_cast<uint32_t>(required_layers.size());
	create_info.ppEnabledLayerNames = required_layers.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(required_extension.size());
	create_info.ppEnabledExtensionNames = required_extension.data();
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
