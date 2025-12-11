//
// Created by Harold on 2025/12/11.
//

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <print>
#include <unordered_map>
#include <unordered_set>

#include "../utils/hash.h"

using device_map_t = std::unordered_map<std::string, std::vector<std::string>>;

template <> struct std::formatter<device_map_t>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const device_map_t& dm, std::format_context& ctx) const
    {
        auto out = ctx.out();
        for (const auto& [device, connections] : dm)
        {
            out = std::format_to(out, "{}: ", device);
            for (size_t i = 0; i < connections.size(); ++i)
            {
                if (i > 0) out = std::format_to(out, " ");
                out = std::format_to(out, "{}", connections[i]);
            }
            out = std::format_to(out, "\n");
        }
        return out;
    }
};

device_map_t parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    device_map_t dm;
    while (std::getline(ifs, line))
    {
        auto p = line.find(':');
        dm[line.substr(0, p)] = line.substr(p + 2) | std::views::split(' ') |
                                std::views::transform([](auto&& sv) { return std::string(std::string_view(sv)); }) |
                                std::ranges::to<std::vector>();
    }
    return dm;
}

long part_1(char const* fn)
{
    auto dm = parse_input(fn);
    //std::println("{}", dm);

    std::unordered_set<std::string> visited;
    long cnt = 0;
    auto find_path = [&dm, &visited, &cnt](auto self, std::string const& current, std::string const& target) -> void {
        visited.insert(current);
        if (current == target)
            cnt++;
        else
            for (auto const& next : dm.at(current))
                if (!visited.contains(next)) self(self, next, target);
        visited.erase(current);
    };
    find_path(find_path, "you", "out");

    return cnt;
}

long long part_2(char const* fn)
{
    auto dm = parse_input(fn);

    std::unordered_map<std::tuple<std::string, bool, bool>, long long> visited;
    auto find_path = [&dm, &visited](auto self, std::string const& current, std::string const& target, bool visited_dac,
                                     bool visited_fft) -> long long {
        visited_dac |= current == "dac";
        visited_fft |= current == "fft";
        if (current == target) return visited_dac && visited_fft;

        auto current_state = std::make_tuple(current, visited_dac, visited_fft);
        if (!visited.contains(current_state))
            visited[current_state] = std::ranges::fold_left(
                dm.at(current), 0LL,
                [&dm, &visited, &self, &target, visited_dac, visited_fft](auto sum, auto const& next) {
                    return sum + self(self, next, target, visited_dac, visited_fft);
                });
        return visited[current_state];
    };
    return find_path(find_path, "svr", "out", false, false);
}

int main(int argc, char* argv[])
{
    std::println("How many different paths lead from you to out?\n{}", part_1(argv[1]));
    std::println("How many of those paths visit both dac and fft?\n{}", part_2(argv[1]));

    return 0;
}