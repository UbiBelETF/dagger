#pragma once


#include "core/core.h"

#include <cmath>
#include <algorithm>
#include <random>

namespace Random
{
    template<typename T>
    class Sequence
    {
        ::Sequence<T> choices;
        UInt32 repeats = 2;

    public:
        Sequence(UInt32 repeats_, ::Sequence<T> choices_)
            : choices{ choices }
            , repeats{ repeats_ }
        {
            assert(choices.size() > 0);
        }

        Sequence(UInt32 repeats_, std::initializer_list<T>&& choices_)
            : choices{ std::move(choices_) }
            , repeats{ repeats_ }
        {
            assert(choices.size() > 0);
        }

        Sequence(::Sequence<T> choices_)
            : choices{ choices }
            , repeats{ 2 }
        {
            assert(choices.size() > 0);
        }

        Sequence(std::initializer_list<T>&& choices_)
            : choices{ std::move(choices_) }
            , repeats{ 2 }
        {
            assert(choices.size() > 0);
        }

        ~Sequence() = default;

    public:
        inline T Get()
        {
            static auto rng = std::default_random_engine{};
            static ::Sequence<T> gen{};

            if (gen.size() < repeats)
            {
                for (int i = 0; i < repeats; i++)
                    for (auto element : choices) gen.push_back(element);

                std::shuffle(std::begin(gen), std::end(gen), rng);
            }

            auto rnd = gen.back();
            gen.pop_back();

            return rnd;
        }
    };

    Float32 Uniform()
    {
        static auto rng = std::default_random_engine{};
        static ::Sequence<Float32> gen{};

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