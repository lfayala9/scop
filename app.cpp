#include "app.hpp"

app::app(uint32_t w, uint32_t h) : width(w), height(h)
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

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enable_validation)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL app::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    (void)messageSeverity;
    (void)messageType;
    (void)pUserData;
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void	app::debug_message()
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

bool	app::is_suitable(vk::raii::PhysicalDevice const &device)
{
	bool	support_13;
	bool	support_extensions;
	bool	support_graphics;
	bool	support_feats;
	auto	queue_families = device.getQueueFamilyProperties();
	auto	available_extensions = device.enumerateDeviceExtensionProperties();
	auto 	features = device.template getFeatures2<vk::PhysicalDeviceFeatures2,
                                                    vk::PhysicalDeviceVulkan11Features,
                                                    vk::PhysicalDeviceVulkan13Features,
                                                    vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
											
	support_13 = device.getProperties().apiVersion >= VK_API_VERSION_1_3;
	support_graphics = std::ranges::any_of(queue_families, [](auto const &qfp) {return !!(qfp.queueFlags &vk::QueueFlagBits::eGraphics);});
	support_extensions = std::ranges::all_of(requiredDeviceExtension, [&available_extensions](auto const &requiredDeviceExtension)
											{
												return std::ranges::any_of(available_extensions, [requiredDeviceExtension](auto const &available_extensions)
												{return strcmp(available_extensions.extensionName, requiredDeviceExtension) == 0;});
											});
	support_feats = features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
                    features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                    features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;
	return (support_13 && support_graphics && support_extensions && support_feats);
}

void	app::get_device()
{
	std::vector<vk::raii::PhysicalDevice>	devices = instance.enumeratePhysicalDevices();
	auto const								dev_iter = std::ranges::find_if(devices, [&](auto const &device){return is_suitable(device);});

	if (dev_iter == devices.end())
		throw std::runtime_error( "failed to find a suitable GPU!" );
	device = std::move(*dev_iter);
	// auto props = device.getProperties();
	// auto mem_props = device.getMemoryProperties();

	// uint64_t vram_bytes = 0;
	// for (uint32_t i = 0; i < mem_props.memoryHeapCount; ++i)
	// {
	// 	if (mem_props.memoryHeaps[i].flags & vk::MemoryHeapFlagBits::eDeviceLocal)
	// 		vram_bytes += mem_props.memoryHeaps[i].size;
	// }

	// std::cout << "\n================ GPU INFO ================" << std::endl;
	// std::cout << "  Name:           " << props.deviceName << std::endl;
	// std::cout << "  Type:           " << vk::to_string(props.deviceType) << std::endl;
	// std::cout << "  Vulkan Version: " << VK_API_VERSION_MAJOR(props.apiVersion) << "."
	//           << VK_API_VERSION_MINOR(props.apiVersion) << "."
	//           << VK_API_VERSION_PATCH(props.apiVersion) << std::endl;
	// std::cout << "  Driver Version: " << props.driverVersion << std::endl;
	// std::cout << "  VRAM (Dedicated): " << (vram_bytes / (1024 * 1024)) << " MB ("
	//           << (static_cast<double>(vram_bytes) / (1024.0 * 1024.0 * 1024.0)) << " GB)" << std::endl;
	// std::cout << "==========================================\n" << std::endl;
}

void	app::instance_vulkan()
{
	VkApplicationInfo	app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
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
    // if (unsupportedLayerIt != required_layers.end())
    //     throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
    if (unsupportedLayerIt != required_layers.end())
    {
        std::cerr << "Validation layers unavailable; continuing without them.\n";
        enable_validation = false;
        required_layers.clear();
    }
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
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledLayerCount = static_cast<uint32_t>(required_layers.size());
	create_info.ppEnabledLayerNames = required_layers.data();
	create_info.enabledExtensionCount = static_cast<uint32_t>(required_extension.size());
	create_info.ppEnabledExtensionNames = required_extension.data();
	instance = vk::raii::Instance(context, create_info);
}

void	app::run()
{
	main_loop();
}

void	app::init_vulkan()
{
	instance_vulkan();
	debug_message();
	get_device();
}

void	app::main_loop()
{
    while (!glfwWindowShouldClose(wdw))
        glfwPollEvents();
}

app::~app()
{
	if (wdw)
    	glfwDestroyWindow(wdw);
    glfwTerminate();	
}
    // if (unsupportedLayerIt != required_layers.end())
    //     throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));