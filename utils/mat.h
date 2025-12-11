#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <format>
#include <compare>
#include <optional>

#include "hash.h"

struct Point
{
    int x, y;

    Point() = default;
    Point(int i, int j): x(i), y(j) {}
    Point(Point const&) = default;
    Point& operator=(Point const&) = default;
    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    auto operator<=>(Point const&) const = default;

    Point& operator-=(Point const& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Point& operator+=(Point const& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    std::size_t hash() const noexcept { return ::hash(x, y); }
};

Point operator+(Point const& p1, Point const& p2)
{
    return Point(p1.x + p2.x, p1.y + p2.y);
}

Point operator-(Point const& p1, Point const& p2)
{
    return Point(p1.x - p2.x, p1.y - p2.y);
}

template <> struct std::formatter<Point>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(Point const& p, format_context& ctx) const { return std::format_to(ctx.out(), "[{}, {}]", p.x, p.y); }
};

template <typename T> class Mat
{
public:
    using value_type = typename std::vector<T>::value_type;
    using size_type = typename std::vector<T>::size_type;

public:
    Mat() = default;

    Mat(size_type const r, size_type const c): rows(r), cols(c) { data.resize(r * c); }

    Mat(size_type const r, size_type const c, value_type const x): rows(r), cols(c) { data.resize(r * c, x); }

    auto&& operator[](this auto&& self, size_type const i, size_type const j) noexcept
    {
        return self.data[i * self.cols + j];
    }

    auto&& operator[](this auto&& self, Point const& p) noexcept { return self.operator[](p.x, p.y); }

    std::vector<value_type> row(size_type const i) const noexcept
    {
        return std::vector<value_type>(data.begin() + i * cols, data.begin() + (i + 1) * cols);
    }

    std::vector<value_type> col(size_type const j) const noexcept
    {
        std::vector<value_type> ret(rows);
        for (auto i = 0; i < rows; i++)
            ret[i] = data[i * cols + j];
        return ret;
    }

    Mat sub_mat(size_type const i, size_type const j, size_type const r, size_type const c) const noexcept
    {
        Mat ret(r, c);
        for (auto k = 0; k < r; k++)
            for (auto l = 0; l < c; l++)
                ret[k, l] = (*this)[i + k, j + l];
        return ret;
    }

    Mat transpose() const noexcept
    {
        Mat ret(cols, rows);
        for (auto i = 0; i < rows; i++)
            for (auto j = 0; j < cols; j++)
                ret[j, i] = (*this)[i, j];
        return ret;
    }

    std::optional<Point> find(T const& val) const noexcept
    {
        for (auto i = 0; i < rows; i++)
            for (auto j = 0; j < cols; j++)
                if ((*this)[i, j] == val) return Point{i, j};
        return {};
    }

    bool is_local_minimum(int const i, int const j) const
    {
        std::vector<Point> neighbors;
        neighbors.reserve(8);
        find_8_neighbors(i, j, neighbors);
        for (auto n : neighbors)
            if (this->operator[](n.x, n.y) <= this->operator[](i, j)) return false;

        return true;
    }

    bool is_local_maximum(int const i, int const j) const
    {
        std::vector<Point> neighbors;
        neighbors.reserve(8);
        find_8_neighbors(i, j, neighbors);
        for (auto n : neighbors)
            if (this->operator[](n.x, n.y) >= this->operator[](i, j)) return false;

        return true;
    }

    bool valid_pos(int const i, int const j) const { return (0 <= i && i < rows) && (0 <= j && j < cols); }

    bool valid_pos(Point const& p) const { return valid_pos(p.x, p.y); }

    void find_8_neighbors(int const i, int const j, std::vector<Point>& neighbors) const
    {
        // eight directions
        if (valid_pos(i - 1, j)) neighbors.push_back(Point(i - 1, j));
        if (valid_pos(i + 1, j)) neighbors.push_back(Point(i + 1, j));
        if (valid_pos(i, j - 1)) neighbors.push_back(Point(i, j - 1));
        if (valid_pos(i, j + 1)) neighbors.push_back(Point(i, j + 1));
        if (valid_pos(i - 1, j - 1)) neighbors.push_back(Point(i - 1, j - 1));
        if (valid_pos(i - 1, j + 1)) neighbors.push_back(Point(i - 1, j + 1));
        if (valid_pos(i + 1, j - 1)) neighbors.push_back(Point(i + 1, j - 1));
        if (valid_pos(i + 1, j + 1)) neighbors.push_back(Point(i + 1, j + 1));
    }

    void find_8_neighbors(Point p, std::vector<Point>& neighbors) const { find_8_neighbors(p.x, p.y, neighbors); }

    void find_4_neighbors(int const i, int const j, std::vector<Point>& neighbors) const
    {
        // four directions
        if (valid_pos(i - 1, j)) neighbors.push_back(Point(i - 1, j));
        if (valid_pos(i + 1, j)) neighbors.push_back(Point(i + 1, j));
        if (valid_pos(i, j - 1)) neighbors.push_back(Point(i, j - 1));
        if (valid_pos(i, j + 1)) neighbors.push_back(Point(i, j + 1));
    }

    void find_4_neighbors(Point p, std::vector<Point>& neighbors) const { find_4_neighbors(p.x, p.y, neighbors); }

public:
    size_type rows{}, cols{};
    std::vector<T> data{};
};

template <typename T> struct std::formatter<Mat<T>>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    auto format(Mat<T> const& mat, format_context& ctx) const
    {
        auto out = ctx.out();
        for (int i = 0; i < mat.rows; ++i)
        {
            for (int j = 0; j < mat.cols; ++j)
            {
                if (j > 0) out = std::format_to(out, " ");
                out = std::format_to(out, "{}", mat[i, j]);
            }
            out = std::format_to(out, "\n");
        }
        return out;
    }
};

void read_all(std::ifstream& ifs, Mat<char>& mat)
{
    std::string line;
    char c;
    int rows = 0, cols = 0;
    bool first = true;
    while (std::getline(ifs, line))
    {
        for (auto c : line)
        {
            mat.data.push_back(c);
            if (first) cols++;
        }
        first = false;
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);
}

void read_all(std::ifstream& ifs, Mat<int>& mat, char base_char = '0')
{
    std::string line;
    char c;
    int rows = 0, cols = 0;
    bool first = true;
    while (std::getline(ifs, line))
    {
        for (auto c : line)
        {
            mat.data.push_back(c - base_char);
            if (first) cols++;
        }
        first = false;
        rows++;
    }
    mat.rows = rows;
    mat.cols = cols;
    mat.data.resize(mat.rows * mat.cols, 0);
}