#include "TestClearColour.h"

#include "Renderer.h"
#include "imgui/imgui.h"
namespace test
{
	TestClearColour::TestClearColour(Renderer* renderer)
		: mClearColour{ 0.2f, 0.3f, 0.8f, 1.0 }, Test(renderer)
	{}

	TestClearColour::~TestClearColour()
	{	}

	void TestClearColour::OnUpdate(float deltaTime)
	{	}

	void TestClearColour::OnRender()
	{
		GLCall(glClearColor(mClearColour[0], mClearColour[1], mClearColour[2], mClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColour::OnImGuiRender()
	{
		ImGui::ColorPicker4("Clear Colour", mClearColour);
	}
}