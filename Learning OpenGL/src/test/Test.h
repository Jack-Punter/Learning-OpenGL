#pragma once
#include <vector>
#include <string>
#include <functional>

#include "Renderer.h"

namespace test
{
	class Test
	{
	public:
		Test() = default;
		Test(Renderer* renderer);
		virtual ~Test() = default;

		virtual void OnUpdate(float deltaTime) { GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)) }
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	
	protected:
		Renderer* renderer;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPtr, Renderer* renderer);
		
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test\n";

			Tests.push_back(std::make_pair(name, [](Renderer* renderer) { return new T(renderer); }));
		}

		void OnImGuiRender() override;

	private:
		Test*& CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*(Renderer*)>>> Tests;
		Renderer* renderer;
	};
}