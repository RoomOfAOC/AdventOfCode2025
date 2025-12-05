//
// Created by Harold on 2025/12/5.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/str_op.h"

int part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int cnt = 0;
    std::vector<std::vector<long long>> fresh_ingredent_ranges;
    while (std::getline(ifs, line))
    {
        if (line.empty()) break;
        fresh_ingredent_ranges.push_back(std::views::split(line, '-') | std::views::transform([](auto&& s) {
                                             return parse_num<long long>(std::string_view(s));
                                         }) |
                                         std::ranges::to<std::vector>());
    }
    while (std::getline(ifs, line))
    {
        auto ingredient = parse_num<long long>(line);
        cnt += std::ranges::any_of(fresh_ingredent_ranges,
                                   [ingredient](auto&& v) { return ingredient >= v[0] && ingredient <= v[1]; });
    }

    return cnt;
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    long long cnt = 0;
    std::vector<std::vector<long long>> fresh_ingredent_ranges;
    while (std::getline(ifs, line))
    {
        if (line.empty()) break;
        fresh_ingredent_ranges.push_back(std::views::split(line, '-') | std::views::transform([](auto&& s) {
                                             return parse_num<long long>(std::string_view(s));
                                         }) |
                                         std::ranges::to<std::vector>());
    }

    std::ranges::sort(fresh_ingredent_ranges);
    auto l = fresh_ingredent_ranges[0][0];
    auto r = fresh_ingredent_ranges[0][1];

    std::ranges::for_each(fresh_ingredent_ranges | std::views::drop(1), [&l, &r, &cnt](auto&& v) {
        auto nl = v[0];
        auto nr = v[1];
        if (nl <= r + 1) // overlap
            r = std::max(r, nr);
        else
        {
            cnt += r - l + 1;
            l = nl;
            r = nr;
        }
    });
    cnt += r - l + 1;

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("How many of the available ingredient IDs are fresh?\n{}", part_1(argv[1]));
    std::println("How many ingredient IDs are considered to be fresh according to the fresh ingredient ID ranges?\n{}",
                 part_2(argv[1]));

    return 0;
}