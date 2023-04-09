#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>

class Application
{
public:
	Application(const std::string& title, int width, int height, bool vsync = true);
	void Run();

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnShutdown() = 0;

	GLFWwindow* GetWindow();
	glm::ivec2 GetWindowSize() { return {m_Width, m_Height};}
	void SetWindowSize(glm::ivec2 size);

private:
	std::string m_Title;
	int m_Width;
	int m_Height;
	bool m_Vsync;
	GLFWwindow* m_Window;

};