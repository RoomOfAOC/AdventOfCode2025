#pragma once

#include <format>

#include "hash.h"

template <typename T> struct Point3D
{
    T x, y, z;

    Point3D() = default;
    Point3D(T i, T j, T k): x(i), y(j), z(k) {}
    Point3D(Point3D const&) = default;
    Point3D& operator=(Point3D const&) = default;
    Point3D(Point3D&&) = default;
    Point3D& operator=(Point3D&&) = default;

    auto operator<=>(Point3D const&) const = default;

    Point3D& operator-=(Point3D const& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Point3D& operator+=(Point3D const& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    T n2() const noexcept { return x * x + y * y + z * z; }

    struct Hasher
    {
        size_t operator()(Point3D const& point) const
        {
            return container_hash<std::initializer_list<T>>()({point.x, point.y, point.z});
        }
    };
};

template <typename T> struct std::formatter<Point3D<T>>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(Point3D<T> const& p, format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}, {}]", p.x, p.y, p.z);
    }
};

template <typename T> Point3D<T> operator+(Point3D<T> const& p1, Point3D<T> const& p2)
{
    return Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

template <typename T> Point3D<T> operator-(Point3D<T> const& p1, Point3D<T> const& p2)
{
    return Point3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}