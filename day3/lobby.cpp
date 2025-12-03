//
// Created by Harold on 2025/12/3.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/str_op.h"

long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    long cnt = 0;
    while (std::getline(ifs, line))
        if (auto it = std::max_element(line.cbegin(), line.cend()); it == line.cend() - 1)
            cnt += (*std::max_element(line.cbegin(), line.cend() - 1) - '0') * 10 + (*it - '0');
        else
            cnt += (*it - '0') * 10 + (*std::max_element(it + 1, line.cend()) - '0');

    return cnt;
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    long long cnt = 0;
    while (std::getline(ifs, line))
    {
        while (line.size() > 12)
        {
            auto it = std::ranges::adjacent_find(line, std::ranges::less());
            if (it == line.end()) it = line.end() - 1;
            line.erase(it);
        }
        cnt += std::ranges::fold_left(line, long long{0}, [](long long a, char b) { return a * 10 + (b - '0'); });
    }

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("what is the total output joltage?\n{}", part_1(argv[1]));
    std::println("What is the new total output joltage?\n{}", part_2(argv[1]));

    return 0;
}