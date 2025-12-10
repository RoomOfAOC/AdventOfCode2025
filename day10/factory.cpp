//
// Created by Harold on 2025/12/10.
//

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <print>
#include <queue>

#include <z3++.h>

#include "../utils/str_op.h"

struct Machine
{
    std::string lights;
    std::vector<std::vector<int>> buttons;
    std::vector<int> joltages;
};

std::vector<Machine> parse_input(char const* fn)
{
    std::ifstream ifs(fn);
    std::string line;
    std::vector<Machine> machines;
    while (std::getline(ifs, line))
    {
        Machine m;
        auto tokens = line | std::views::split(' ') |
                      std::views::transform([](auto&& s) { return std::string_view(s); }) |
                      std::ranges::to<std::vector>();

        for (const auto& token : tokens)
        {
            auto content = token.substr(1, token.length() - 2);
            if (token.starts_with('['))
                m.lights = content;
            else if (token.starts_with('('))
                m.buttons.push_back(content | std::views::split(',') | std::views::transform([](auto&& x) {
                                        return parse_num<int>(std::string_view(x));
                                    }) |
                                    std::ranges::to<std::vector>());
            else if (token.starts_with('{'))
                m.joltages = content | std::views::split(',') |
                             std::views::transform([](auto&& x) { return parse_num<int>(std::string_view(x)); }) |
                             std::ranges::to<std::vector>();
        }
        machines.push_back(m);

        // std::println("{}", m.lights);
        // std::println("{}", m.buttons);
        // std::println("{}", m.joltages);
    }
    return machines;
}

// BFS
int min_presses_for_machine(const Machine& machine)
{
    int n = machine.lights.size();
    int m = machine.buttons.size();

    std::vector<int> presses(1 << n, -1);
    int initial_state = 0;
    int target_state = 0;

    for (int i = 0; i < n; i++)
        if (machine.lights[i] == '#') target_state |= (1 << i);

    presses[initial_state] = 0;
    std::queue<int> q;
    q.push(initial_state);

    while (!q.empty())
    {
        auto current_state = q.front();
        q.pop();

        if (current_state == target_state) return presses[current_state];

        for (auto button_idx = 0; button_idx < m; button_idx++)
        {
            auto next_state = current_state;
            // toggle lights
            for (auto light_idx : machine.buttons[button_idx])
                if (light_idx < n) next_state ^= (1 << light_idx);

            if (presses[next_state] == -1)
            {
                presses[next_state] = presses[current_state] + 1;
                q.push(next_state);
            }
        }
    }

    return -1;
}

long part_1(char const* fn)
{
    auto machines = parse_input(fn);
    long total_presses = 0;

    for (const auto& machine : machines)
    {
        auto presses = min_presses_for_machine(machine);
        if (presses == -1)
        {
            std::println("something wrong with machine\n{}\n{}\n{}", machine.lights, machine.buttons, machine.joltages);
            return -1;
        }
        total_presses += presses;
    }

    return total_presses;
}

int min_presses_for_machine_2(const Machine& machine)
{
    int n = machine.joltages.size();
    int m = machine.buttons.size();

    z3::context context{};
    z3::optimize solver(context);

    // button vars
    std::vector<z3::expr> button_presses;
    for (auto i = 0; i < m; i++)
    {
        auto var_name = "b" + std::to_string(i);
        auto var = context.int_const(var_name.c_str());
        button_presses.push_back(var);
        solver.add(var >= 0);
    }

    // counter constraints
    for (auto counter_idx = 0; counter_idx < n; counter_idx++)
    {
        z3::expr sum = context.int_val(0);

        for (auto button_idx = 0; button_idx < m; button_idx++)
        {
            auto affect_count =
                std::count(machine.buttons[button_idx].begin(), machine.buttons[button_idx].end(), counter_idx);

            if (affect_count > 0) sum = sum + button_presses[button_idx] * context.int_val(affect_count);
        }

        // joltage constraint
        solver.add(sum == context.int_val(machine.joltages[counter_idx]));
    }

    // objective
    z3::expr objective = context.int_val(0);
    for (auto i = 0; i < m; i++)
        objective = objective + button_presses[i];

    solver.minimize(objective);

    // check
    auto result = solver.check();
    if (result != z3::sat) return -1; // no solution

    // solve
    auto model = solver.get_model();
    auto total_presses = 0;
    for (auto i = 0; i < m; i++)
    {
        auto val = model.eval(button_presses[i]);
        int press_count = val.get_numeral_int();
        total_presses += press_count;
    }

    return total_presses;
}

long part_2(char const* fn)
{
    auto machines = parse_input(fn);
    long total_presses = 0;

    for (const auto& machine : machines)
    {
        auto presses = min_presses_for_machine_2(machine);
        if (presses == -1)
        {
            std::println("something wrong with machine\n{}\n{}\n{}", machine.lights, machine.buttons, machine.joltages);
            return -1;
        }
        total_presses += presses;
    }

    return total_presses;
}

int main(int argc, char* argv[])
{
    std::println(
        "What is the fewest button presses required to correctly configure the indicator lights on all of the machines?\n{}",
        part_1(argv[1]));
    std::println(
        "What is the fewest button presses required to correctly configure the indicator lights on all of the machines?\n{}",
        part_2(argv[1]));

    return 0;
}