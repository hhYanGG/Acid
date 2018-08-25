﻿#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const float UiInputButton::CHANGE_TIME = 0.1f;
	const float UiInputButton::FONT_SIZE = 1.7f;
	const Vector2 UiInputButton::DIMENSION = Vector2(0.36f, 0.05f);
	const float UiInputButton::SCALE_NORMAL = 1.0f;
	const float UiInputButton::SCALE_SELECTED = 1.1f;

	UiInputButton::UiInputButton(UiObject *parent, const Vector2 &position, const std::string &string, const TextJustify &justify) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre", true), FONT_SIZE, string, FontType::Resource("Fonts/ProximaNova", "Regular"), justify, DIMENSION.m_x)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_mouseOver(false)
	{
	}

	UiInputButton::~UiInputButton()
	{
		delete m_text;
		delete m_background;
	}

	void UiInputButton::UpdateObject()
	{
		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}
}
