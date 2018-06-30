#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <random>

#include "Test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{
	class FlappyTest : public Test
	{
	private:
		static constexpr float wndWidth = 960.0f;
		static constexpr float wndHeight = 540.0f;

		glm::mat4 proj = glm::ortho(0.0f, wndWidth, 0.0f, wndHeight, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		static constexpr float backgroundVerteces[]{
			0.0f,   0.0f,   0.0f, 0.0f,
			960.0f, 0.0f,   1.0f, 0.0f,
			960.0f, 540.0f, 1.0f, 1.0f,
			0.0f,   540.0f, 0.0f, 1.0f
		};
		
		struct Position
		{
			Position() = default;

			inline Position operator+(const Position& rhs)
			{
				return { x + rhs.x, y + rhs.y, z + rhs.z };
			}

			inline Position operator-(const Position& rhs)
			{
				return { x - rhs.x, y - rhs.y, z - rhs.z };
			}

			inline Position& operator+=(const Position& rhs)
			{
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
				return *this;
			}

			inline Position& operator-=(const Position& rhs)
			{
				x -= rhs.x;
				y -= rhs.y;
				z -= rhs.z;
				return *this;
			}

			inline Position(float x, float y, float z = 0.0f)
				: x(x), y(y), z(z)
			{	}

			float x, y, z;
		};

		class Pipe
		{
		private:
			glm::mat4 proj = glm::ortho(0.0f, wndWidth, 0.0f, wndHeight, -1.0f, 1.0f);
			glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			static constexpr float vertexs[] = {
				-25.0f, -270.0f, 0.0f, 0.0f,
				 25.0f, -270.0f, 1.0f, 0.0f,
				 25.0f,  270.0f, 1.0f, 1.0f,
				-25.0f,  270.0f, 0.0f, 1.0f
			};
			static constexpr unsigned int indeces[] = {
				0, 1, 2,
				2, 3, 0,
			};

			static constexpr float velocity = 200.0f;
			static constexpr auto shaderDir = "res/shaders/Basic.shader";
			static constexpr auto textureDir = "res/textures/PipeTexture.png";
			friend class FlappyTest;

		public:
			Pipe(Renderer* renderer, Shader& shader, VertexArray& va, IndexBuffer& ib);
			Pipe(const Pipe& src);
			Pipe(Pipe&& src) noexcept;
			Pipe& operator=(const Pipe& src);
			Pipe& operator=(Pipe&&) noexcept = default;
			~Pipe() = default;

			void OnUpdate(float deltaTime);
			void Render();
			FlappyTest::Position Pos = { 0.0f, wndHeight / 2.0f };

		private:
			Renderer* renderer;
			Shader& shader;
			VertexArray& va;
			IndexBuffer& ib;

			glm::mat4 topMVP;
			glm::mat4 bottomMVP;

			Position offset = { 0.0f, wndHeight / 2.0f + 100.0f, 0.0f };
		};
		
		friend class Pipe;

		static constexpr float birdWidth = 34.0f;
		static constexpr float birdHeight = 24.0;

		static constexpr float birdVerteces[] = {
			-birdWidth / 2.0f, -birdHeight / 2.0f, 0.0f, 0.0f,
			 birdWidth / 2.0f, -birdHeight / 2.0f, 1.0f, 0.0f,
			 birdWidth / 2.0f,  birdHeight / 2.0f, 1.0f, 1.0f, 
			-birdWidth / 2.0f,  birdHeight / 2.0f, 0.0f, 1.0f
		};
		static constexpr unsigned int birdIndeces[] = {
			0, 1, 2,
			2, 3, 0,
		};

		static constexpr auto birdShaderDir = "res/shaders/Basic.shader";
		static constexpr auto wingUpTextureDir = "res/textures/upflap.png";
		static constexpr auto wingMidTextureDir = "res/textures/midflap.png";
		static constexpr auto wingDownTextureDir = "res/textures/downflap.png";
		static constexpr auto backgroundTextureDir = "res/textures/Background.png";

		static constexpr unsigned int pipeTexturePos = 0;
		static constexpr unsigned int wingUpTexturePos = 1;
		static constexpr unsigned int wingMidTexturePos = 2;
		static constexpr unsigned int wingDownTexturePos = 3;
		static constexpr unsigned int backgroundTexturePos = 4;
	public:
		FlappyTest(Renderer* renderer);

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		IndexBuffer pipeIb;
		Shader pipeShader;
		VertexBuffer pipeVb;
		VertexBufferLayout pipeVbLayout;
		VertexArray pipeVa;
		Texture pipeTexture;

		Shader birdShader;
		VertexBuffer birdVb;
		IndexBuffer birdIb;
		VertexBufferLayout birdVbLayout;
		VertexArray birdVa;
		Texture wingUp;
		Texture wingMid;
		Texture wingDown;

		IndexBuffer backgroundIb;
		VertexBuffer backgroundVb;
		VertexBufferLayout backgroundVbLayout;
		VertexArray backgroundVa;
		Texture backgroundTexture;

	private:
		bool isColliding();

		std::random_device rd;
		std::mt19937 rng = std::mt19937(rd());
		std::uniform_int_distribution<int> pipeHeight = std::uniform_int_distribution<int>(150, 340);

		glm::mat4 MVP;
		Position birdPos = { 150.0f, 270.0f };
		float birdAcceleration = -600.0f;
		float birdVelocity = 0.0f;
		int score = 0;
		bool hasScored = false;

		std::vector<Pipe> pipes;
		const int nPipes = 7;
		float frontXPos = 900.0f;
	};
}
