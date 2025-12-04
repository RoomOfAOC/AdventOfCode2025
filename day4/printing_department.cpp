//
// Created by Harold on 2025/12/4.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>

#include "../utils/mat.h"

long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<char> m;
    read_all(ifs, m);
    //std::println("{}", m);

    int cnt = 0;
    std::vector<Point> neighbors;
    neighbors.reserve(8);
    for (auto i = 0uz; i < m.rows; i++)
        for (auto j = 0uz; j < m.cols; j++)
        {
            if (m[i, j] != '@') continue;

            neighbors.clear();
            m.find_8_neighbors(i, j, neighbors);
            if (neighbors.size() < 4 ||
                std::ranges::count_if(neighbors, [&m](auto const& p) { return m[p] == '@'; }) < 4)
                cnt++;
        }

    return cnt;
}

size_t part_2(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<char> m;
    read_all(ifs, m);

    std::vector<Point> neighbors;
    neighbors.reserve(8);
    std::vector<Point> visited;
    visited.reserve(m.rows * m.cols);
    size_t cnt = 0;
    while (true)
    {
        visited.clear();
        for (auto i = 0uz; i < m.rows; i++)
            for (auto j = 0uz; j < m.cols; j++)
            {
                if (m[i, j] != '@') continue;

                neighbors.clear();
                m.find_8_neighbors(i, j, neighbors);
                if (neighbors.size() < 4 ||
                    std::ranges::count_if(neighbors, [&m](auto const& p) { return m[p] == '@'; }) < 4)
                    visited.emplace_back(Point(i, j));
            }
        if (visited.empty()) break;
        cnt += visited.size();
        for (auto const& p : visited)
            m[p] = '.';
    }

    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("How many rolls of paper can be accessed by a forklift?\n{}", part_1(argv[1]));
    std::println("How many rolls of paper in total can be removed by the Elves and their forklifts?\n{}",
                 part_2(argv[1]));

    return 0;
}