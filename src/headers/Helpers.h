#pragma once

#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <filesystem>

#include <exception>

#include <vector>
#include <optional>
#include <memory>
#include <algorithm>

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using u_ptr = std::unique_ptr<T>;

using String = std::string;
using StringView = std::string_view;
using Path = std::filesystem::path;
using CharMatrix = Vector<Vector<char>>;

template <typename T>
struct Vector4 
{
	T x{}; T y{}; T z{}; T w{};

	Vector4 operator+(const Vector4& other) const
	{
		Vector4 result = *this;
		result.x += other.x;
		result.y += other.y;
		result.z += other.z;
		result.w += other.w;
		return result;
	}
}; 