#pragma once

#include "types.h"

namespace foundation
{
	template<typename T>
	struct Vector2
	{
		T x, y;
	};

	template<typename T>
	struct Vector3
	{
		T x, y, z;
	};

	template<typename T>
	struct Vector4
	{
		T x, y, z, w;
	};

	template<typename T>
	struct Quaternion
	{
		T x, y, z, w;
	};

	template<typename T>
	struct Matrix3x3
	{
		Vector3 x, y, z;
	};

	template<typename T>
	struct Matrix4x4
	{
		Vector4 x, y, z, t;
	};

	template<typename T>
	struct AABB
	{
		Vector3 min, max;
	};

	template<typename T>
	struct OOBB
	{
		Matrix4x4 tm;
		AABB aabb;
	};

	typedef Vector2<float> vec2f;
	typedef Vector3<float> vec3f;
	typedef Vector4<float> vec4f;
	typedef Quaternion<float> quatf;
	typedef Matrix3x3<float> mat3f;
	typedef Matrix4x4<float> mat4f;
}