/// @file    matrix.h
/// @date    2019-02-24 17:38:08

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <vector>
#include <initializer_list>

template <typename Object>
class Matrix
{
	public:
		Matrix(int rows, int cols)
			:array_(rows, std::vector<Object>(cols, Object{}))
		{}

		Matrix(std::initializer_list<std::vector<Object>> lst)
			:array_(lst.size())
		{
			int i = 0;
			for(auto& v : lst)
				array_[i++] = std::move(v);
		}

		Matrix(const std::vector<std::vector<Object>>& v)
			:array_{v}
		{}

		Matrix(std::vector<std::vector<Object>>&& v)
			:array_{std::move(v)}
		{}

		~Matrix() = default;

		const std::vector<Object>& operator[] (int row) const
		{
			return array_[row];
		}

		std::vector<Object>& operator[] (int row)
		{
			return array_[row];
		}

		std::size_t numrows() const
		{
			return array_.size();
		}

		std::size_t numcols() const
		{
			return numrows() ? array_[0].size() : 0;
		}

	private:
		std::vector<std::vector<Object>> array_;
};

template <typename Object>
std::ostream& operator<< (std::ostream& os, const Matrix<Object>& mat)
{
	os << '[' << std::endl;
	for(std::size_t row = 0; row < mat.numrows(); ++row)
	{
		os << "  " << '[';
		for(std::size_t col = 0; col < mat.numcols(); ++col)
		{
			os << mat[row][col] << ' ';
		}
		os << ']' << std::endl;
	}
	os << ']';
	return os;
}

#endif
