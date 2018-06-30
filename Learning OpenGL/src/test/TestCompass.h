#pragma once
#include "test/Test.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>

namespace test
{
	class TestCompass : public Test
	{
	private:
		Shader shader;
		Texture texture;
		VertexBuffer vb;
		VertexBufferLayout layout;
		VertexArray va;
		IndexBuffer ib;
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		float translate[3];

	public:
		TestCompass(Renderer* rendererPtr);
		~TestCompass();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		static constexpr auto shaderDir = "res/shaders/Basic.shader";
		static constexpr auto textureDir = "res/textures/Compass_Card_transparent.png";

		static constexpr float vertexes[] = {
			-50, -50, 0, 0, //0
			 50, -50, 1, 0, //1
			 50,  50, 1, 1, //2
			-50,  50, 0, 1  //3
		};

		static constexpr unsigned int indexes[] = {
			0, 1, 2,
			2, 3, 0
		};
	};
}