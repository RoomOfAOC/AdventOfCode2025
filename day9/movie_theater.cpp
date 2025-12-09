//
// Created by Harold on 2025/12/9.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/str_op.h"
#include "../utils/point2d.h"

template <typename T> T rect_area(Point2D<T> const& p1, Point2D<T> const& p2)
{
    return (std::abs(p1.x - p2.x) + 1) * (std::abs(p1.y - p2.y) + 1);
}

long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<Point2D<long long>> points;
    while (std::getline(ifs, line))
    {
        auto xy = std::views::split(line, ',') |
                  std::views::transform([](auto&& s) { return parse_num<long long>(std::string_view(s)); }) |
                  std::ranges::to<std::vector>();
        points.emplace_back(xy[0], xy[1]);
    }
    long long max = 0;
    for (auto i = 0uz; i < points.size(); i++)
        for (auto j = i + 1; j < points.size(); j++)
            max = std::max(max, rect_area(points[i], points[j]));

    return max;
}

template <typename T>
bool is_segments_intersect(Point2D<T> const& p1, Point2D<T> const& p2, Point2D<T> const& p3, Point2D<T> const& p4)
{
    auto orientation = [](Point2D<T> const& a, Point2D<T> const& b, Point2D<T> const& c) -> int {
        auto val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
        if (val == T{0}) return 0;
        return (val > T{0}) ? -1 : 1;
    };

    auto o1 = orientation(p1, p2, p3);
    auto o2 = orientation(p1, p2, p4);
    auto o3 = orientation(p3, p4, p1);
    auto o4 = orientation(p3, p4, p2);

    return o1 * o2 < 0 && o3 * o4 < 0;
}

template <typename T>
bool is_rect_inside_polygon(Point2D<T> const& p1, Point2D<T> const& p2, std::vector<Point2D<T>> const& polygon)
{
    auto corners = std::vector{p1, Point2D<T>(p2.x, p1.y), p2, Point2D<T>(p1.x, p2.y)};
    if (!std::ranges::all_of(corners, [&polygon](auto const& p) { return is_point_inside_polygon(p, polygon); }))
        return false;

    // U / C shape
    auto edges = std::vector{std::pair(corners[0], corners[1]), std::pair(corners[1], corners[2]),
                             std::pair(corners[2], corners[3]), std::pair(corners[3], corners[0])};
    for (auto i = 0uz; i < polygon.size(); i++)
    {
        auto const& p1 = polygon[i];
        auto const& p2 = polygon[(i + 1) % polygon.size()];

        if (std::ranges::any_of(edges, [&p1, &p2](auto const& edge) {
                auto const& [ep1, ep2] = edge;
                return is_segments_intersect(ep1, ep2, p1, p2);
            }))
            return false;
    }
    return true;
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<Point2D<long long>> points;
    while (std::getline(ifs, line))
    {
        auto xy = std::views::split(line, ',') |
                  std::views::transform([](auto&& s) { return parse_num<long long>(std::string_view(s)); }) |
                  std::ranges::to<std::vector>();
        points.emplace_back(xy[0], xy[1]);
    }
    long long max = 0;
    for (auto i = 0uz; i < points.size(); i++)
        for (auto j = i + 1; j < points.size(); j++)
        {
            auto p1 = Point2D<long long>{std::min(points[i].x, points[j].x), std::min(points[i].y, points[j].y)};
            auto p2 = Point2D<long long>{std::max(points[i].x, points[j].x), std::max(points[i].y, points[j].y)};
            if (is_rect_inside_polygon(p1, p2, points)) max = std::max(max, rect_area(p1, p2));
        }

    return max;
}

int main(int argc, char* argv[])
{
    std::println("what is the largest area of any rectangle you can make?\n{}", part_1(argv[1]));
    std::println("what is the largest area of any rectangle you can make using only red and green tiles?\n{}",
                 part_2(argv[1]));

    return 0;
}