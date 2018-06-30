#include "TestPentagon.h"
#include "imgui/imgui.h"
#include <algorithm>

namespace test
{

	TestPentagon::TestPentagon(Renderer* renderer)
		:
		shader(shaderDir), ib(index, indexCount), vb(vertexes, vertexSize),
		layout(), renderer(renderer), va(), transform{100.0f, 100.0f, 0}
	{
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
	}

	TestPentagon::~TestPentagon()
	{}

	void TestPentagon::OnUpdate(float deltaTime)
	{
		shader.Bind();
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

		glm::mat4 mvp = proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(transform[0], transform[1], transform[2]));
		shader.SetUniformMat4f("uMVP", mvp);
	}

	void TestPentagon::OnRender()
	{
		renderer->Draw(va, ib, shader);
	}

	void TestPentagon::OnImGuiRender()
	{
		ImGui::SliderFloat3("Transform ", transform, 0, 960);
	}

}