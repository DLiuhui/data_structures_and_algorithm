/// @file    IntCell.h

#ifndef __INTCELL_H__
#define __INTCELL_H__

/**
 * 一个模拟整数单元的类
 */
class IntCell
{
	public:
		// 使用explicit禁止隐式类型转换
		// int->IntCell 是禁止的
		explicit IntCell(int initialValue = 0);
		int read() const;
		void write(int x);
	
	private:
		int storedValue_;
};

// version 2
class IntCellPtr
{
	public:
		explicit IntCellPtr(int initialValue = 0);
		~IntCellPtr();
		IntCellPtr(const IntCellPtr& rhs);
		IntCellPtr(IntCellPtr&& rhs);
		IntCellPtr& operator= (const IntCellPtr& rhs);
		IntCellPtr& operator= (IntCellPtr&& rhs);
		int read() const;
		void write(int x);
	private:
		int *storedValue_;
};
#endif
