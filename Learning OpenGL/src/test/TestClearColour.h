#pragma once
#include "Test.h"

namespace test
{

	class TestClearColour : public Test
	{
	public:
		TestClearColour(Renderer*);
		~TestClearColour();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float mClearColour[4];

	};
}