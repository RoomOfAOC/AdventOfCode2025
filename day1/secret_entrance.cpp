//
// Created by Harold on 2025/12/1.
//

#include <fstream>
#include <string>
#include <print>

#include "../utils/str_op.h"

int part_1(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int r = 50;
    int cnt = 0;
    while (std::getline(ifs, line))
    {
        r = (line[0] == 'L') ? (100 + (r - parse_num<int>(line.substr(1))) % 100) % 100 :
                               (r + parse_num<int>(line.substr(1))) % 100;

        if (r == 0) cnt++;
    }
    return cnt;
}

int part_2(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    int r = 50;
    int cnt = 0;
    while (std::getline(ifs, line))
    {
        auto dr = parse_num<int>(line.substr(1));
        cnt += dr / 100;
        auto rr = dr % 100;
        if (line[0] == 'L')
        {
            auto n_r = (100 + r - rr) % 100;
            if (r < n_r && r != 0) cnt++;
            r = n_r;
        }
        else
        {
            auto n_r = (r + rr) % 100;
            if (r > n_r && n_r != 0) cnt++;
            r = n_r;
        }

        if (r == 0) cnt++;
    }
    return cnt;
}

int main(int argc, char* argv[])
{
    std::println("What's the actual password to open the door?\n{}", part_1(argv[1]));
    std::println("What's the actual password to open the door?\n{}", part_2(argv[1]));
    return 0;
}