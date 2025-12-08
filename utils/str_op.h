#pragma once

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <charconv>
#include <ranges>

auto ltrim(std::string_view sv, std::string_view trim_sv = " ")
{
    auto it = std::ranges::find_if_not(sv, [trim_sv](unsigned char c) { return trim_sv.contains(c); });
    return sv.substr(std::distance(sv.begin(), it));
}

auto rtrim(std::string_view sv, std::string_view trim_sv = " ")
{
    auto rit =
        std::ranges::find_if_not(sv | std::views::reverse, [trim_sv](unsigned char c) { return trim_sv.contains(c); });
    return sv.substr(0, sv.size() - std::distance(sv.rbegin(), rit));
}

auto strip(std::string_view sv, std::string_view trim_sv = " ")
{
    return rtrim(ltrim(sv, trim_sv), trim_sv);
}

template <typename T> T parse_num(std::string_view token)
{
    T num{};
    std::from_chars(token.data(), token.data() + token.size(), num);
    return num;
}

template <typename T> T parse_hex_num(std::string_view token)
{
    T num{};
    std::from_chars(token.data(), token.data() + token.size(), num, 16);
    return num;
}