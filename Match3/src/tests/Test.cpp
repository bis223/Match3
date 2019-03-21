#include "Test.h"
#include "imgui/imgui.h"

namespace test
{
	Test::Test()
	{
	}

	TestMenu::TestMenu(Test*& m_currentTestPointer)
		: m_currentTest(m_currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test: m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_currentTest = test.second();
			}
		}
	}

}