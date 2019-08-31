/// @file  Vector.h

#ifndef __VECTOR_H__
#define __VECTOR_H__

#define CHECK

#include <cstdio>
#include <algorithm>

#include "../lib/dsexceptions.h"

namespace DS
{
	template <typename Object>
	class Vector
	{
    public:
        // const params
        static const unsigned int SPACE_CAPACITY = 16;
        // typedef
        typedef unsigned int size_t;

        explicit Vector(size_t init_size = 0):
            the_size_{init_size}, the_capacity_{init_size + SPACE_CAPACITY},
            array_{new Object[the_capacity_]}
        {}

        Vector(const Vector& rhs):
            the_size_{rhs.the_size_}, the_capacity_{rhs.the_capacity_},
            array_{new Object[the_capacity_]}
        {
            for(size_t idx = 0; idx < the_size_; ++idx)
                array_[idx] = rhs.array_[idx];
        }

        Vector& operator= (const Vector& rhs)
        {
            Vector copy = rhs;
            std::swap(*this, copy);
            printf("Call copy =\n");
            return *this;
        }

        Vector(Vector&& rhs) noexcept:
            the_size_{rhs.the_size_}, the_capacity_{rhs.the_capacity_},
            array_{rhs.array_}
        {
            // 由于是移动构造函数，所以rhs得array被完全交付给新的类对象
            // 需要将临时类对象rhs.array_置空
            rhs.array_ = nullptr;
        }

        Vector& operator= (Vector&& rhs) noexcept
        {
            // 由于是移动赋值，当前对象得成员值被交换到临时对象rhs
            // 交换结束后，rhs.array_不置空，因为需要rhs得析构函数去清理rhs的array_
            std::swap(the_size_, rhs.the_size_);
            std::swap(the_capacity_, rhs.the_capacity_);
            std::swap(array_, rhs.array_);
            printf("Call move =\n");
            return *this;
        }

        ~Vector()
        { delete []array_; }

        // other items
        void resize(size_t new_size)
        {
            if(new_size > the_capacity_)
                reserve(new_size * 2);
            the_size_ = new_size;
        }

        void reserve(size_t new_capacity)
        {
            if(new_capacity < the_size_)
                return;

            Object* new_array = new Object[new_capacity];
            for(size_t idx = 0; idx < the_size_; ++idx)
                new_array[idx] = std::move(array_[idx]);

            the_capacity_ = new_capacity;
            std::swap(array_, new_array);
            delete []new_array;
        }

        // functions
        Object& operator[](size_t index)
        {
#ifdef CHECK
            if(index > size())  // 检查上界溢出
                throw ArrayIndexOutOfBoundsException{ };
#endif
            return array_[index];
        }

        const Object& operator[](size_t index) const
        {
#ifdef CHECK
            if(index > size())  // 检查上界溢出
                throw ArrayIndexOutOfBoundsException{ };
#endif
            return array_[index];
        }

        bool empty() const
        { return size() == 0; }

        size_t size() const
        { return the_size_; }

        int capacity() const
        { return the_capacity_; }

        void push_back(const Object& x)
        {
            if(the_size_ == the_capacity_)
                reserve(2 * the_capacity_ + 1);
            array_[the_size_++] = x;
        }

        void push_back(Object&& x)
        {
            if(the_size_ == the_capacity_)
                reserve(2 * the_capacity_ + 1);  // +1考虑到end()和cend()指向最后一个元素的后一位
            array_[the_size_++] = std::move(x);
        }

        void pop_back()  // 并非真正的弹出，而是size标志-1
        {
            if(empty())  // 检查下界溢出
                throw UnderflowException{ };
            --the_size_;
        }

        const Object& back() const
        {
            if(empty())  // 检查下界溢出
                throw UnderflowException{ };
            return array_[the_size_ - 1]; }

        typedef Object* iterator;
        typedef const Object* const_iterator;

        iterator begin()
        { return &array_[0]; }
        const_iterator cbegin() const
        { return &array_[0]; }
        iterator end()  // 返回最后一个元素的后一个
        { return &array_[size()]; }
        const_iterator cend() const
        { return &array_[size()]; }


    private:
        size_t the_size_;
        size_t the_capacity_;
        Object* array_;
	};
}

#endif
