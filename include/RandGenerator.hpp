#pragma once
#include <random>

template <class T>
class RandGenerator
{
    public:
        RandGenerator(T min_value, T max_value) : gen(std::random_device{}()), dist(min_value, max_value)
        {

        }   
        T generateValue() 
        {
            return dist(gen);
        }
    private:
        std::mt19937 gen;
		std::uniform_real_distribution<T> dist;
};