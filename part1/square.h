/// @file    square.h

#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <iostream>
#include <vector>

class Square
{
	public:
		explicit Square(double s = 0.0)
			:side_{s}
		{}

		double getSide() const
		{ return side_; }

		double getArea() const
		{ return side_ * side_; }

		double getPerimeter() const
		{ return 4 * side_; }

		void print(std::ostream& os = std::cout) const
		{ os << "(square" << getSide() << ")"; }

		bool operator< (const Square& rhs) const
		{ return getSide() < rhs.getSide(); }

		bool operator> (const Square& rhs) const
		{ return getSide() > rhs.getSide(); }

	private:
		double side_;
};

std::ostream& operator<< (std::ostream& out, const Square& rhs)
{
	rhs.print(out);
	return out;
}

// swap -> 使用move机制
void mySwap(Square& lhs, Square& rhs)
{
	Square tmp = static_cast<Square &&>(lhs);
	lhs = static_cast<Square &&>(rhs);
	rhs = static_cast<Square &&>(tmp);
}

template <typename Comparable>
const Comparable& findMax(const std::vector<Comparable>& a)
{
	int maxIdx = 0;
	for(std::size_t i = 0; i < a.size(); ++i)
	{
		if(a[maxIdx] < a[i])
			maxIdx = i;
	}
	return a[maxIdx];
}

#endif
