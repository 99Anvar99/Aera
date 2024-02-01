#pragma once
#include "commands/math.h"

class lerping_float
{
public:
	lerping_float(float start_current, float speed) : m_target(start_current), m_current(start_current),
	                                                  m_speed(speed / 10.f)
	{
	}

	lerping_float() = default;

	lerping_float& update(const float speed = 0.f)
	{
		if (speed != 0.f)
		{
			m_speed = speed;
			if (!math::are_floats_equal(m_speed, speed / 10.f))
			{
				m_speed /= 10.f;
			}
		}

		if (std::abs(m_current - m_target) > 0.0005)
		{
			m_current = m_current * (1 - m_speed) + m_target * m_speed;
		}
		else
		{
			m_current = m_target;
		}

		return *this;
	}

	lerping_float& set_target(float new_target)
	{
		m_target = new_target;
		return *this;
	}

	lerping_float& set_current(float new_current)
	{
		m_current = new_current;
		return *this;
	}

	float get_current()
	{
		return m_current;
	}

private:
	float m_target{};
	float m_current{};
	float m_speed{};
};
