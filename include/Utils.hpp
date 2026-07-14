#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

namespace utils 
{

	int getIndexToCoord(int x, int y, int len_line) 
	{
		return y * len_line + x;
	}
	std::pair<int,int> getCoordToIndex(int index, int len_line)
	{
		int y = index / len_line;
		int x = index % len_line;

		return std::pair<int, int>{x, y};
	} 
	bool checkBounds(int y, int x, int max_y, int max_x)
	{
		return (x >= 0 && x < max_x) && (y >= 0 && y < max_y);
	}
	bool checkBounds(int index, int min, int max)
	{
		return (index >= min && index < max);
	}
	
	template<typename T>
	bool parseNumber(const std::string& s, T& out) 
	{
		try 
		{
			if constexpr (std::is_same_v<T, int>) 
			{
				out = std::stoi(s);
			} 
			else if constexpr (std::is_same_v<T, float>) 
			{
				out = std::stof(s);
			} 
			else if constexpr (std::is_same_v<T, double>) 
			{
				out = std::stod(s);
			} 
			else 
			{
				static_assert(sizeof(T) == 0, "Type not supported");
			}
			return true;
		} 
		catch (const std::exception& e) 
		{
			return false;
		}
	}
	template<class T>
	T writeData(std::string name_param, T default_value)
	{
		std::string buf;
		std::string str = "Write " + name_param + "[" + std::to_string(default_value) + "]: ";
		std::cout << str;
		std::getline(std::cin, buf);
		T value;
		value = parseNumber<T>(buf, value) ? value : default_value;
		return value;
	}
	template<typename T>
	bool in(const T& value, const std::vector<T>& vec)
	{
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}
}