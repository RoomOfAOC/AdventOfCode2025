//
// Created by Harold on 2025/12/9.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>
#include <unordered_map>
#include <cassert>

#include "../utils/str_op.h"
#include "../utils/point3d.h"

template <typename T> struct DisjointSet
{

    std::vector<T> parent;
    std::vector<T> rank;

    DisjointSet(T n)
    {
        assert(n > 0);

        parent.reserve(n);
        for (auto i = T{0}; i < n; i++)
            parent.push_back(i);
        rank.resize(n, T{1});
    }

    T find(T x)
    {
        assert(x >= 0 && x < parent.size());

        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(T x, T y)
    {
        auto root_x = find(x);
        auto root_y = find(y);

        if (root_x != root_y)
        {
            if (rank[root_x] < rank[root_y]) std::swap(root_x, root_y);

            parent[root_y] = root_x;
            rank[root_x] += rank[root_y];
        }
    }

    T get_rank(T x) { return rank[find(x)]; }
};

size_t part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<Point3D<long long>> points;
    while (std::getline(ifs, line))
    {
        auto xyz = std::views::split(line, ',') |
                   std::views::transform([](auto&& s) { return parse_num<long long>(std::string_view(s)); }) |
                   std::ranges::to<std::vector>();
        points.emplace_back(xyz[0], xyz[1], xyz[2]);
    }

    std::vector<std::pair<std::pair<size_t, size_t>, long long>> distances;
    for (auto i = 0uz; i < points.size(); i++)
        for (auto j = i + 1; j < points.size(); j++)
            distances.emplace_back(std::make_pair(i, j), (points[i] - points[j]).n2());
    std::ranges::sort(distances, [](const auto& p1, auto const& p2) { return p1.second > p2.second; });
    // for (auto&& [p, d] : distances)
    //     std::println("{} -> {} = {}", points[p.first], points[p.second], d);

    DisjointSet<size_t> ds(points.size());
    for (auto i = 0; i < 1000; i++)
    {
        auto [pair, distance] = distances.back();
        distances.pop_back();
        // std::println("{} -> {} = {}", points[pair.first], points[pair.second], distance);
        ds.unite(pair.first, pair.second);
    }
    auto rank = ds.rank;
    std::ranges::sort(rank, std::greater{});
    // std::println("{}", rank);

    return rank[0] * rank[1] * rank[2];
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<Point3D<long long>> points;
    while (std::getline(ifs, line))
    {
        auto xyz = std::views::split(line, ',') |
                   std::views::transform([](auto&& s) { return parse_num<long long>(std::string_view(s)); }) |
                   std::ranges::to<std::vector>();
        points.emplace_back(xyz[0], xyz[1], xyz[2]);
    }

    std::vector<std::pair<std::pair<size_t, size_t>, long long>> distances;
    for (auto i = 0uz; i < points.size(); i++)
        for (auto j = i + 1; j < points.size(); j++)
            distances.emplace_back(std::make_pair(i, j), (points[i] - points[j]).n2());
    std::ranges::sort(distances, [](const auto& p1, auto const& p2) { return p1.second > p2.second; });

    DisjointSet<size_t> ds(points.size());
    while (true)
    {
        auto [pair, distance] = distances.back();
        distances.pop_back();
        auto [i, j] = pair;
        ds.unite(i, j);
        if (ds.get_rank(i) == points.size()) return points[i].x * points[j].x;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    std::println("what do you get if you multiply together the sizes of the three largest circuits?\n{}",
                 part_1(argv[1]));
    std::println(
        "What do you get if you multiply together the X coordinates of the last two junction boxes you need to connect?\n{}",
        part_2(argv[1]));

    return 0;
}