//
// Created by Harold on 2025/12/2.
//

#include <fstream>
#include <string>
#include <ranges>
#include <print>

#include "../utils/str_op.h"

long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    //std::println("line: {}", line);
    auto IDs = std::views::split(line, ',') | std::views::transform([](auto&& s) {
                   return std::views::split(s, '-') |
                          std::views::transform([](auto&& ss) { return parse_num<long long>(std::string_view(ss)); }) |
                          std::ranges::to<std::vector>();
               }) |
               std::ranges::to<std::vector>();

    //std::println("IDs: {}", IDs);

    auto is_invalid_id = [](auto id) {
        auto s = std::to_string(id);
        auto ss = s + s;
        return ss.size() / ss.find(s, 1) % 4 == 0;
    };

    long long cnt = 0;
    for (auto const& id : IDs)
        for (auto i = id[0]; i < id[1] + 1; i++)
            cnt += is_invalid_id(i) ? i : 0;

    return cnt;
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::getline(ifs, line);
    auto IDs = std::views::split(line, ',') | std::views::transform([](auto&& s) {
                   return std::views::split(s, '-') |
                          std::views::transform([](auto&& ss) { return parse_num<long long>(std::string_view(ss)); }) |
                          std::ranges::to<std::vector>();
               }) |
               std::ranges::to<std::vector>();

    auto is_invalid_id = [](auto id) {
        auto s = std::to_string(id);
        auto ss = s + s;
        return ss.size() / ss.find(s, 1) > 2;
    };

    long long cnt = 0;
    for (auto const& id : IDs)
        for (auto i = id[0]; i < id[1] + 1; i++)
            cnt += is_invalid_id(i) ? i : 0;

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("What do you get if you add up all of the invalid IDs?\n{}", part_1(argv[1]));
    std::println("What do you get if you add up all of the invalid IDs using these new rules?\n{}", part_2(argv[1]));

    return 0;
}