#include "ContentExit.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.4375f), "Centre", true, false, Vector2(0.8f, 0.7f))),
		m_background(new Gui(this, GetRectangle(), Texture::Resource("Guis/White.png"))),
		m_button1(new UiInputButton(m_background, Vector2(0.5f, 0.4375f), "Test Button", JUSTIFY_CENTRE)),
		m_input1(new UiInputGrabber(m_background, Vector2(0.5f, 0.38f), "Test Grabber: ", MOUSE_BUTTON_1, new UiGrabberMouse(), JUSTIFY_CENTRE)),
		m_slider1(new UiInputSlider(m_background, Vector2(0.5f, 0.3225f), "Test Slider: ", 1, 0.0f, 100.0f, 50.0f, JUSTIFY_CENTRE)),
		m_text1(new UiInputText(m_background, Vector2(0.5f, 0.265f), "Test Text: ", "Hello World", 14, JUSTIFY_CENTRE))
	{
	}

	ContentExit::~ContentExit()
	{
		delete m_background;
		delete m_button1;
		delete m_input1;
		delete m_slider1;
	}

	void ContentExit::UpdateObject()
	{
	}
}
