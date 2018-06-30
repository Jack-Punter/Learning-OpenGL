#include "Test.h"
#include "imgui/imgui.h"

namespace test
{

	Test::Test(Renderer* renderer)
		: renderer(renderer)
	{}

	TestMenu::TestMenu(Test*& currentTestPtr, Renderer* renderer)
		: CurrentTest(currentTestPtr), renderer(renderer)
	{}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				CurrentTest = test.second(renderer);
		}
	}
}