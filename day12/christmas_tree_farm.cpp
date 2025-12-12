//
// Created by Harold on 2025/12/12.
//

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/str_op.h"

struct shape_t
{
    int width, height;
    std::vector<std::string> grid;
};

struct region_t
{
    int width, height;
    std::vector<int> shapes;
};

using input_t = std::pair<std::vector<shape_t>, std::vector<region_t>>;
template <> struct std::formatter<input_t>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(input_t const& in, std::format_context& ctx) const
    {
        auto out = ctx.out();
        auto const& [shapes, regions] = in;
        auto i = 0;
        for (auto const& shape : shapes)
        {
            if (i++ > 0) out = std::format_to(out, "\n");
            out = std::format_to(out, "{}x{}: {}", shape.width, shape.height, shape.grid);
        }
        for (auto const& region : regions)
        {
            out = std::format_to(out, "\n");
            out = std::format_to(out, "{}x{}: {}", region.width, region.height, region.shapes);
        }
        out = std::format_to(out, "\n");
        return out;
    }
};

input_t parse_input(char const* fn)
{
    using std::operator""sv;

    std::ifstream ifs(fn);
    input_t input{};
    std::string txt{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    auto tokens = txt | std::views::split("\n\n"sv) |
                  std::views::transform([](auto&& s) { return std::string_view(s); }) | std::ranges::to<std::vector>();
    for (auto i = 0uz; i < tokens.size() - 1; i++)
        input.first.push_back(
            shape_t{.width = 3,
                    .height = 3,
                    .grid = tokens[i].substr(tokens[i].find('\n') + 1) | std::views::split('\n') |
                            std::views::transform([](auto&& s) { return std::string(std::string_view(s)); }) |
                            std::ranges::to<std::vector>()});
    std::ranges::for_each(
        tokens.back() | std::views::split('\n') | std::views::transform([](auto&& s) { return std::string_view(s); }),
        [&input](auto&& s) {
            auto x = s.find('x');
            auto colon = s.find(':', x);
            auto w = parse_num<int>(s.substr(0, x));
            auto h = parse_num<int>(s.substr(x + 1, colon));
            input.second.push_back(
                region_t{.width = w,
                         .height = h,
                         .shapes = s.substr(colon + 2) | std::views::split(' ') |
                                   std::views::transform([](auto&& s) { return parse_num<int>(std::string_view(s)); }) |
                                   std::ranges::to<std::vector>()});
        });
    return input;
}

long long part_1(char const* fn)
{
    auto input = parse_input(fn);
    //std::println("{}", input);

    // backtrace to fit every shape with 8 transformations is too slow
    long long cnt = 0;
    std::ranges::for_each(input.second, [&input, &cnt](auto const& region) {
        if (std::ranges::fold_left(
                std::views::enumerate(region.shapes) | std::views::transform([&input, &region](auto&& p) {
                    auto const& [i, n] = p;
                    auto const& shape = input.first[i];
                    return std::ranges::count(std::views::join(shape.grid) | std::ranges::to<std::string>(), '#') * n;
                }),
                0LL, std::plus{}) <= region.width * region.height)
            cnt++;
    });

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("How many of the regions can fit all of the presents listed?\n{}", part_1(argv[1]));

    return 0;
}