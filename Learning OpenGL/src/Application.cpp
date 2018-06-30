#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <memory>
#include <chrono>


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "test/TestClearColour.h"
#include "test/TestCompass.h"
#include "test/TestPentagon.h"
#include "test/FlappyTest.h"

/*
RAII Initializer for GLFW so that our class destructors
don't get stuck in a loop trying to clear GL errors due
to an uninitialized GL context
*/
struct glfwInitializer
{
	bool state;
	glfwInitializer()
	{
		state = glfwInit();
	}
	~glfwInitializer()
	{
		if (state)
			glfwTerminate();
	}
	operator bool() { return state; }
};

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	glfwInitializer glfwState = glfwInitializer();
	if (!glfwState)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << '\n';

	std::cout << glGetString(GL_VERSION) << '\n';

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	Renderer renderer;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();


	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest, &renderer);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestClearColour>("Clear Colour");
	testMenu->RegisterTest<test::TestCompass>("Compass");
	testMenu->RegisterTest<test::TestPentagon>("Pentagon");
	testMenu->RegisterTest<test::FlappyTest>("Flappy");
	
	/* Loop until the user closes the window */
	float frameTime = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();
		
		if (currentTest)
		{
			currentTest->OnUpdate(frameTime);
			currentTest->OnRender();
			ImGui::Begin("Test");
			
			if (currentTest != testMenu && ImGui::Button("<-"))
			{
				delete currentTest;
				currentTest = testMenu;
			}

			currentTest->OnImGuiRender();
			ImGui::End();
		}
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> frameDuration(t2 - t1);
		frameTime = frameDuration.count();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	delete currentTest;
	if (currentTest != testMenu)
		delete testMenu;
	
	return 0;
}