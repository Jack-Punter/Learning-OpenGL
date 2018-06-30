#pragma once
#include "test/Test.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

namespace test
{
	class TestPentagon : public Test
	{
	private:
		static constexpr float vertexes[] = {
			 0.00f,  100.0f, //0
			-95.1f,  30.9f, //1
			-58.8f, -80.9f, //2
			 58.8f, -80.9f, //3
			 95.1f,  30.9f, //4
		};
		static constexpr unsigned int  index[] = {
			0, 1, 2,
			0, 2, 3, 
			0, 3, 4,			
		};
		
		static constexpr unsigned int indexCount = 9;
		static constexpr unsigned int vertexSize = 5 * 2 * sizeof(float);

		Shader shader;
		IndexBuffer ib;
		VertexBuffer vb;
		VertexBufferLayout layout;
		Renderer* renderer;
		VertexArray va;

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		float transform[3];
	
	public:
		TestPentagon(Renderer* renderer);
		~TestPentagon();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	
	private:
		static constexpr auto shaderDir = "res/shaders/PlainCol.shader";
	};
}
