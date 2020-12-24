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
        ::Sequence<T> m_Choices{};
        ::Sequence<T> m_Generator{};
        UInt32 m_Repeats = 2;

    public:
        Sequence(UInt32 repeats_, ::Sequence<T> choices_)
            : m_Choices{ m_Choices }
            , m_Generator{}
            , m_Repeats{ repeats_ }
        {
            assert(m_Choices.size() > 0);
        }

        Sequence(UInt32 repeats_, std::initializer_list<T>&& choices_)
            : m_Choices{ std::move(choices_) }
            , m_Generator{}
            , m_Repeats{ repeats_ }
        {
            assert(m_Choices.size() > 0);
        }

        Sequence(::Sequence<T> choices_)
            : m_Choices{ m_Choices }
            , m_Generator{}
            , m_Repeats{ 2 }
        {
            assert(m_Choices.size() > 0);
        }

        Sequence(std::initializer_list<T>&& choices_)
            : m_Choices{ std::move(choices_) }
            , m_Generator{}
            , m_Repeats{ 2 }
        {
            assert(m_Choices.size() > 0);
        }

        Sequence(const Sequence<T>& other_)
            : m_Choices{ other_.m_Choices }
            , m_Generator{ other_.m_Generator }
            , m_Repeats{ other_.m_Repeats }
        {
            assert(m_Choices.size() > 0);
        }

        Sequence(const Sequence<T>& other_, ::Sequence<T> pregen_)
            : m_Choices{ other_.m_Choices }
            , m_Generator{ pregen_ }
            , m_Repeats{ other_.m_Repeats }
        {
            assert(m_Choices.size() > 0);
        }

        ~Sequence() = default;

    public:

        inline ::Sequence<T> GetPool() const
        {
            return m_Generator;
        }

        inline T Get()
        {
            static auto rng = std::default_random_engine{};
            
            if (m_Generator.size() < m_Repeats)
            {
                for (int i = 0; i < m_Repeats; i++)
                    for (auto element : m_Choices) m_Generator.push_back(element);

                std::shuffle(std::begin(m_Generator), std::end(m_Generator), rng);
            }

            auto rnd = m_Generator.back();
            m_Generator.pop_back();

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

    Float32 Uniform(Float32 min_, Float32 max_)
    {
        if (max_ > min_)
        {
            Float32 tmp = min_;
            min_ = max_;
            max_ = tmp;
        }

        return min_ + Uniform() * (max_ - min_);
    }

    Vector2 InCircle()
    {
        static Float64 PI2 = 2 * 3.14159265358979323846264;
        Float64 theta = PI2 * Random::Uniform();
        Float64 u = (Float64)Random::Uniform() + (Float64)Random::Uniform();
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