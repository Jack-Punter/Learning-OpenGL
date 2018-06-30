#include "TestCompass.h"
#include "imgui/imgui.h"

namespace test
{

	TestCompass::TestCompass(Renderer* rendererPtr)
		: shader(shaderDir), texture(textureDir), vb(vertexes, 4 * 4 * sizeof(float)),
		  ib(indexes, 6), Test(rendererPtr), translate{ 100, 100, 0 }
	{
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		texture.Bind();
	}

	TestCompass::~TestCompass()
	{	}

	void TestCompass::OnUpdate(float deltaTime)
	{
		shader.Bind();
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

		glm::mat4 mvp = proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(translate[0], translate[1], translate[2]));
		
		shader.SetUniformMat4f("uMVP", mvp);
		shader.SetUniform1i("uTexture", 0);
	}

	void TestCompass::OnRender()
	{
		renderer->Draw(va, ib, shader);
	}

	void TestCompass::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translate", translate, 0.0f, 960.0f);
	}

}