#include "Application.h"

Application::Application(const std::string& title, int width, int height, bool vsync)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;
	m_Vsync = vsync;
	spdlog::set_level(spdlog::level::trace);

}

void Application::Run()
{
	if (!glfwInit())
	{
		LOG_CRITICAL("Failed to init GLFW");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//	printf("GLFW version: %s\n", glfwGetVersionString());
	LOG_TRACE("GLFW version: {}", glfwGetVersionString());
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

	if (m_Window == nullptr)
	{
		LOG_CRITICAL("Failed to create GLFW window");
		return;
	}

	glfwSetWindowUserPointer(m_Window, reinterpret_cast<void *>(this));

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
	{
		auto* data = static_cast<Application*>(glfwGetWindowUserPointer(window));

		data->m_Width = width;
		data->m_Height = height;

		glViewport(0, 0, width, height);

		LOG_TRACE("Resized {} x {}", data->m_Width, data->m_Height);
	});

	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_CRITICAL("Failed to init GLAD");
		return;
	}

	LOG_TRACE("OpenGL version: {}", *glGetString(GL_VERSION));
	glfwSwapInterval(m_Vsync);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	OnInit();

	while (!glfwWindowShouldClose(m_Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		OnUpdate();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	OnShutdown();
	glfwDestroyWindow(m_Window);
	glfwTerminate();

}

GLFWwindow* Application::GetWindow()
{
	return m_Window;
}

void Application::SetWindowSize(glm::ivec2 size)
{
	glfwSetWindowSize(m_Window,size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}
