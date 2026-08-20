#include "window.hpp"

window::window(uint32_t w, uint32_t h): WIDTH(w), HEIGHT(h)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	wdw = glfwCreateWindow(WIDTH, HEIGHT, "AAAA", nullptr, nullptr);
	main_loop();
}

void	window::main_loop()
{
    while (!glfwWindowShouldClose(wdw))
        glfwPollEvents();
}

window::~window()
{
    glfwDestroyWindow(wdw);
    glfwTerminate();	
}
