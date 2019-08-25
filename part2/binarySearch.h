/// @file    binarySearch.h

#ifndef __BINARYSEARCH_H__
#define __BINARYSEARCH_H__

#include <vector>

#define NOT_FOUND -1
// return index, if not found, return -1
	template <typename Object>
int binarySearch(const std::vector<Object>& array, const Object& key)
{
	std::size_t low = 0, high = array.size() - 1;
	std::size_t mid = low + (high - low) / 2;
	while(low <= high)                      
	{                                      
		if(array[mid] > key)                  
			high = mid - 1;
		else if(array[mid] < key)
			low = mid + 1;
		else
			return mid;
		mid = low + (high - low) / 2;
	}
	return NOT_FOUND;
}
#endif
