/// @file    IntCell.cpp

#include "IntCell.h"
#include <iostream>

/* construct */
IntCell::IntCell(int initialValue):
	storedValue_{initialValue}
{}

/* read */
int IntCell::read() const
{
	return storedValue_;
}

/* stored */
void IntCell::write(int x)
{
	storedValue_ = x;
}

// version 2
// construction
IntCellPtr::IntCellPtr(int initialValue):
	storedValue_{new int{initialValue}}
{}
// deconstructor
IntCellPtr::~IntCellPtr()
{
	delete storedValue_;
}
// copy
IntCellPtr::IntCellPtr(const IntCellPtr& rhs):
	storedValue_{new int{*rhs.storedValue_}}
{}
// move
IntCellPtr::IntCellPtr(IntCellPtr&& rhs):
	storedValue_{rhs.storedValue_}
{
	rhs.storedValue_ = nullptr;
}
// = copy
IntCellPtr& IntCellPtr::operator= (const IntCellPtr& rhs)
{
	if(this != &rhs) // self copy
		*storedValue_ = *rhs.storedValue_;
	return *this;
}
// = move
IntCellPtr& IntCellPtr::operator= (IntCellPtr&& rhs)
{
	std::swap(storedValue_, rhs.storedValue_);
	return *this;
}
// read
int IntCellPtr::read() const
{
	return *storedValue_;
}
// store
void IntCellPtr::write(int x)
{
	*storedValue_ = x;
}

