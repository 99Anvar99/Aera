#pragma once
#include "pch/pch.h"

namespace math
{
	inline float degrees_to_radians(const float degrees)
	{
		//Formula: degrees * PI/180 = y rad
		return static_cast<float>(static_cast<long double>(degrees) * pi / 180.L);
	}

	inline float radians_to_degrees(const float radians)
	{
		//Formula: radians * 180/PI = y deg
		return static_cast<float>(static_cast<long double>(radians) * 180.L / pi);
	}

	inline float distance_3d(Vector3 a, Vector3 b)
	{
		return static_cast<float>(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
	}

	inline Vector3 rotation_to_direction(const Vector3& rot)
	{
		const float x_radian{degrees_to_radians(rot.x)};
		const float z_radian{degrees_to_radians(rot.z)};
		const float absolute_x_radian{fabs(cos(x_radian))};
		return {-sin(z_radian) * absolute_x_radian, cos(z_radian) * absolute_x_radian, sin(x_radian)};
	}

	inline float distance_between_vectors(Vector3 a, Vector3 b)
	{
		return static_cast<float>(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
	}

	template <typename T>
	bool are_numbers_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
	{
		return fabs(a - b) < epsilon;
	}

	inline bool are_floats_equal(const float a, const float b,
	                             const float epsilon = std::numeric_limits<float>::epsilon())
	{
		return fabs(a - b) < epsilon;
	}

	inline bool are_ints_equal(const int a, const int b)
	{
		return a == b;
	}

	inline bool compare_strings(const std::string& str1, const std::string& str2)
	{
		return str1 == str2;
	}

	inline bool compare_chars(const char* str1, const char* str2)
	{
		return str1 == str2;
	}
}
