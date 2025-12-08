//
// Created by Harold on 2025/12/8.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/mat.h"
#include "../utils/str_op.h"

long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<long long> m;
    std::string line;
    while (std::getline(ifs, line))
    {
        auto sv = strip(line);
        if (!std::isdigit(static_cast<unsigned char>(sv[0]))) continue;
        std::ranges::for_each(sv | std::views::split(' '), [&m](auto&& s) {
            auto ssv = std::string_view(s);
            if (!ssv.empty()) m.data.push_back(parse_num<long long>(ssv));
        });
        m.rows += 1;
    }
    m.cols = m.data.size() / m.rows;

    auto i = 0uz;
    return *std::ranges::fold_left_first(
        strip(line) | std::views::split(' ') | std::views::transform([&m, &i](auto&& s) {
            auto ssv = strip(std::string_view(s));
            return ssv.empty() ? 0ll : *std::ranges::fold_left_first(m.col(i++), [ssv](auto&& a, auto&& b) {
                return (ssv == "+") ? a + b : a * b;
            });
        }),
        std::plus{});
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<char> m;
    read_all(ifs, m);
    //std::println("{}", m);

    auto cnt = 0uz;
    for (auto i = 0uz, s = 0uz; i < m.cols; i++)
    {
        auto col = m.col(i);
        if (i == m.cols - 1 || std::ranges::all_of(col, [](auto&& c) { return c == ' '; }))
        {
            auto sub_m = m.sub_mat(0, s, m.rows - 1, i == m.cols - 1 ? i - s + 1 : i - s);
            auto op = m[m.rows - 1, s];
            //std::println("{}, {}x{}, {}", sub_m, sub_m.rows, sub_m.cols, op);

            std::vector<long long> nums;
            nums.reserve(sub_m.cols);
            for (auto c = 0uz; c < sub_m.cols; c++)
            {
                auto col = sub_m.col(c);
                std::string_view sv(col.data(), col.size());
                auto x = parse_num<long long>(strip(sv));
                nums.push_back(x);
            }
            //std::println("{}", nums);

            cnt +=
                *std::ranges::fold_left_first(nums, [op](auto&& a, auto&& b) { return (op == '+') ? a + b : a * b; });

            s = i + 1;
        }
    }

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("What is the grand total found by adding together all of the answers to the individual problems?\n{}",
                 part_1(argv[1]));
    std::println("What is the grand total found by adding together all of the answers to the individual problems?\n{}",
                 part_2(argv[1]));

    return 0;
}