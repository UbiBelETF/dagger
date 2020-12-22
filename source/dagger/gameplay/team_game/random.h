#pragma once


#include "core/core.h"

#include <cmath>
#include <algorithm>
#include <random>

namespace Random
{
    Float32 Uniform()
    {
        static auto rng = std::default_random_engine{};
        static Sequence<Float32> gen{};

        if (gen.size() < 3)
        {
            for (UInt32 c = 0; c < 3; c++)
            {
                for (UInt32 i = 0; i < 10; i++)
                {
                    gen.push_back(((Float32)i / 10.0f) + ((Float32)rand() / RAND_MAX) / 10.0f);
                }
            }

            std::shuffle(std::begin(gen), std::end(gen), rng);
        }

        Float32 result = gen.back();
        gen.pop_back();

        return result;
    }

    Vector2 InCircle()
    {
        static Float64 PI2 = 2 * 3.14159265358979323846264;
        Float64 theta = PI2 * Random::Uniform();
        Float64 u = Random::Uniform() + Random::Uniform();
        Float64 r = (u > 1) ? 2 - u : u;

        return Vector2{ r * cos(theta), r * sin(theta) };
    }

    Vector2 OnCircle()
    {
        static double PI2 = 2 * 3.14159265358979323846264;
        double theta = PI2 * Random::Uniform();
        return Vector2{ cos(theta), sin(theta) };
    }

    Vector2 InEllipse(Vector2 size_)
    {
        Vector2 circle = InCircle();
        return Vector2{ circle.x * size_.x, circle.y * size_.y };
    }

    Vector2 OnEllipse(Vector2 size_)
    {
        Vector2 circle = OnCircle();
        return Vector2{ circle.x * size_.x, circle.y * size_.y };
    }
};