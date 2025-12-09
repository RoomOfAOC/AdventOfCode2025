//
// Created by Harold on 2025/12/9.
//

#include <fstream>
#include <string>
#include <algorithm>
#include <ranges>
#include <print>
#include <queue>
#include <vector>

#include "../utils/mat.h"

long long part_1(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<char> m;
    read_all(ifs, m);
    //std::println("{}, {}x{}", m, m.rows, m.cols);
    auto start = *m.find('S');
    //std::println("{}", start);

    long long cnt = 0;

    std::queue<Point> beams;
    beams.push(start);

    for (auto r : std::views::iota(start.x + 1, static_cast<int>(m.rows)))
    {
        std::queue<Point> new_beams;
        while (!beams.empty())
        {
            auto b = beams.front();
            beams.pop();
            b.x = r;
            if (m[b] == '.')
            {
                new_beams.push(b);
                m[b] = '|';
            }
            else if (m[b] == '^')
            {
                bool split = false;
                for (auto p : {Point{b.x, b.y - 1}, Point{b.x, b.y + 1}})
                    if (m.valid_pos(p))
                    {
                        new_beams.push(p);
                        split = true;
                        m[p] = '|';
                    }
                if (split) cnt += 1;
            }
        }
        beams = std::move(new_beams);
    }
    //std::println("{}", m);

    return cnt;
}

long long part_2(char const* fn)
{
    std::ifstream ifs(fn);
    Mat<char> m;
    read_all(ifs, m);
    auto start = *m.find('S');

    std::vector<long long> timelines(m.cols, 0);
    timelines[start.y] = 1;

    std::queue<Point> beams;
    beams.push(start);

    for (auto r : std::views::iota(start.x + 1, static_cast<int>(m.rows)))
    {
        std::queue<Point> new_beams;
        while (!beams.empty())
        {
            auto b = beams.front();
            beams.pop();
            b.x = r;
            if (m[b] == '.')
            {
                new_beams.push(b);
                m[b] = '|';
            }
            else if (m[b] == '^')
            {
                bool split = false;
                for (auto p : {Point{b.x, b.y - 1}, Point{b.x, b.y + 1}})
                    if (m.valid_pos(p))
                    {
                        new_beams.push(p);
                        split = true;
                        m[p] = '|';
                        timelines[p.y] += timelines[b.y];
                    }
                if (split) timelines[b.y] = 0;
            }
        }
        beams = std::move(new_beams);
    }
    //std::println("{}", timelines);

    return *std::ranges::fold_left_first(timelines, std::plus{});
}
int main(int argc, char* argv[])
{
    std::println("How many times will the beam be split?\n{}", part_1(argv[1]));
    std::println("In total, how many different timelines would a single tachyon particle end up on?\n{}",
                 part_2(argv[1]));

    return 0;
}