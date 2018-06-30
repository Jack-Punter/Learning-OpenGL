#include "FlappyTest.h"

namespace test
{
	/*============================Pipe============================*/
	FlappyTest::Pipe::Pipe(Renderer* renderer, Shader& shader, VertexArray& va, IndexBuffer& ib)
		: renderer(renderer), shader(shader), va(va), ib(ib)
	{	}

	FlappyTest::Pipe::Pipe(const Pipe& src)
		: va(src.va), shader(src.shader), ib(src.ib), Pos(src.Pos)
	{	}

	FlappyTest::Pipe::Pipe(Pipe&& src) noexcept
		: va(src.va), shader(src.shader), ib(src.ib), Pos(std::move(src.Pos))
	{	}

	test::FlappyTest::Pipe& FlappyTest::Pipe::operator=(const Pipe& src)
	{
		va = src.va;
		shader = src.shader;
		ib = src.ib;
		Pos = src.Pos;
		return *this;
	}

	void FlappyTest::Pipe::OnUpdate(float deltaTime)
	{
		Position top = Pos + offset;
		Position bottom = Pos - offset;

		topMVP = proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(top.x, top.y, top.z));
		bottomMVP = proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(bottom.x, bottom.y, bottom.z));
	}

	void FlappyTest::Pipe::Render()
	{
		shader.Bind();
		shader.SetUniform1i("uTexture", FlappyTest::pipeTexturePos);
		shader.SetUniformMat4f("uMVP", topMVP);
		renderer->Draw(va, ib, shader);
		shader.SetUniformMat4f("uMVP", bottomMVP);
		renderer->Draw(va, ib, shader);
	}

	/*============================FlappyTest============================*/
	FlappyTest::FlappyTest(Renderer* renderer)
		: Test(renderer), pipeShader(Pipe::shaderDir), pipeVb(Pipe::vertexs, 4 * 4 * sizeof(float)), pipeIb(Pipe::indeces, 6),
		birdShader(birdShaderDir), birdVb(birdVerteces, 4 * 4 * sizeof(float)), birdIb(birdIndeces, 6), pipeTexture(Pipe::textureDir),
		wingDown(wingDownTextureDir), wingMid(wingMidTextureDir), wingUp(wingUpTextureDir), backgroundVb(backgroundVerteces, 4 * 4 * sizeof(float)),
		backgroundIb(birdIndeces, 6), backgroundTexture(backgroundTextureDir)
	{
		pipeVbLayout.Push<float>(2);
		pipeVbLayout.Push<float>(2);
		pipeVa.AddBuffer(pipeVb, pipeVbLayout);

		pipes.reserve(nPipes);
		for (int i = 0; i < nPipes; i++)
		{
			pipes.emplace_back(renderer, pipeShader, pipeVa, pipeIb);
			pipes.back().Pos.y = float(pipeHeight(rng));
		}
		pipeTexture.Bind(pipeTexturePos);
		backgroundTexture.Bind(backgroundTexturePos);
		//bird
		wingDown.Bind(wingDownTexturePos);
		wingUp.Bind(wingUpTexturePos);
		wingMid.Bind(wingMidTexturePos);

		birdVbLayout.Push<float>(2);
		birdVbLayout.Push<float>(2);
		birdVa.AddBuffer(birdVb, birdVbLayout);

		backgroundVbLayout.Push<float>(2);
		backgroundVbLayout.Push<float>(2);
		backgroundVa.AddBuffer(backgroundVb, backgroundVbLayout);
	}

	void FlappyTest::OnUpdate(float deltaTime)
	{
		if (!isColliding())
		{
			frontXPos -= Pipe::velocity * deltaTime;
			if (frontXPos <= -100.0f)
			{
				frontXPos += 200;

				std::rotate(pipes.begin(), pipes.begin() + 1, pipes.end());
				pipes.back().Pos.y = float(pipeHeight(rng));
				hasScored = false;
			}

			std::for_each(
				pipes.begin(), pipes.end(),
				[=, count = 0](Pipe& pipe) mutable
				{
					pipe.Pos.x = count * 200 + frontXPos;
					pipe.OnUpdate(deltaTime);
					count++;
				}
			);

			//Bird
			birdPos.y += birdVelocity * deltaTime;
			birdVelocity += birdAcceleration * deltaTime;

			MVP = proj * view * glm::translate(glm::mat4(1.0f), glm::vec3(birdPos.x, birdPos.y, birdPos.z));

			//bird
			birdShader.Bind();
			birdShader.SetUniformMat4f("uMVP", MVP);

			if (frontXPos < 150 && score == 0)
			{
				score += 1;
			}

			if (frontXPos < -50 && hasScored == false)
			{
				score += 1;
				hasScored = true;
			}
		}
	}

	void FlappyTest::OnRender()
	{
		birdShader.Bind();
		birdShader.SetUniformMat4f("uMVP", proj * view);
		birdShader.SetUniform1i("uTexture", backgroundTexturePos);
		renderer->Draw(backgroundVa, backgroundIb, birdShader);
		birdShader.SetUniformMat4f("uMVP", MVP);


		std::for_each(pipes.begin(), pipes.end(), [](Pipe& pipe) { pipe.Render(); });

		//bird
		birdShader.Bind();
		if (birdVelocity < -25.0f)
			birdShader.SetUniform1i("uTexture", wingUpTexturePos);
		else if (birdVelocity < 25.0f)
			birdShader.SetUniform1i("uTexture", wingMidTexturePos);
		else
			birdShader.SetUniform1i("uTexture", wingDownTexturePos);

		renderer->Draw(birdVa, birdIb, birdShader);
	}

	void FlappyTest::OnImGuiRender()
	{
		//bird
		ImGui::Button(std::to_string(score).c_str(), { 100, 50 });
		if (isColliding())
		{
			ImGui::Button("Game Over!", { 100, 50 });
		}
		else
		{
			if (ImGui::Button("Jump", { 100, 50 }))
			{
				if (birdVelocity <= -75.0f)
					birdVelocity = 300.0f;
			}
		}
	}

	bool FlappyTest::isColliding()
	{
		bool hitsTopOrBottom = birdPos.y <= birdHeight / 2.0f || birdPos.y >= wndHeight - birdHeight / 2.0f;

		bool hitsPipes = std::any_of(
			pipes.begin(), pipes.end(),
			[=](const Pipe& pipe) {
				return (
					birdPos.x + birdWidth / 2.0f >= pipe.Pos.x - 25.0f &&
					birdPos.x - birdWidth / 2.0f <= pipe.Pos.x + 25.0f) &&
					(birdPos.y + birdHeight / 2.0f >= pipe.Pos.y + 100.0f ||
					birdPos.y - birdHeight / 2.0f <= pipe.Pos.y - 100.0f
				);
			}
		);

		return hitsTopOrBottom || hitsPipes;
	}
}