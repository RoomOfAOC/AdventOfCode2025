#pragma once

#include <format>

#include "hash.h"

template <typename T> struct Point2D
{
    T x, y;

    Point2D() = default;
    Point2D(T i, T j): x(i), y(j) {}
    Point2D(Point2D const&) = default;
    Point2D& operator=(Point2D const&) = default;
    Point2D(Point2D&&) = default;
    Point2D& operator=(Point2D&&) = default;

    auto operator<=>(Point2D const&) const = default;

    Point2D& operator-=(Point2D const& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Point2D& operator+=(Point2D const& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D& operator+=(T const n)
    {
        x += n;
        y += n;
        return *this;
    }

    struct Hasher
    {
        size_t operator()(Point2D const& point) const
        {
            return container_hash<std::initializer_list<T>>()({point.x, point.y});
        }
    };
};

template <typename T> struct std::formatter<Point2D<T>>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(Point2D<T> const& p, format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[{}, {}]", p.x, p.y);
    }
};

template <typename T> Point2D<T> operator+(Point2D<T> const& p1, Point2D<T> const& p2)
{
    return Point2D(p1.x + p2.x, p1.y + p2.y);
}

template <typename T> Point2D<T> operator-(Point2D<T> const& p1, Point2D<T> const& p2)
{
    return Point2D(p1.x - p2.x, p1.y - p2.y);
}

template <typename T> T operator^(Point2D<T> const& p1, Point2D<T> const& p2)
{
    return p1.x * p2.y - p1.y * p2.x;
}

template <typename T> T operator*(Point2D<T> const& p1, Point2D<T> const& p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

template <typename T> bool is_point_on_line(Point2D<T> const& px, Point2D<T> const& p1, Point2D<T> const& p2)
{
    return static_cast<T>((p1 - px) ^ (p2 - px)) == T{0};
}

template <typename T> bool is_point_on_segment(Point2D<T> const& px, Point2D<T> const& p1, Point2D<T> const& p2)
{
    return is_point_on_line(px, p1, p2) && (px - p1) * (px - p2) <= T{0};
}

template <typename T> bool is_point_inside_polygon(Point2D<T> const& px, std::vector<Point2D<T>> const& polygon)
{
    // not polygon
    if (polygon.size() < 3) return false;

    bool inside = false;
    for (auto i = 0uz; i < polygon.size(); i++)
    {
        auto const& p1 = polygon[i];
        auto const& p2 = polygon[(i + 1) % polygon.size()];

        // on edge
        if (is_point_on_segment(px, p1, p2)) return true;

        // intersection with polygon edge
        if (p1.y != p2.y && (p1.y > px.y) != (p2.y > px.y) &&
            (px.x < p1.x + (px.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y)))
            inside = !inside;
    }

    return inside;
}
