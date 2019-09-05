//
// Created by DDRHb on 2019/8/31.
//

#ifndef __STACK_H__
#define __STACK_H__

#include "List.h"
#include <iostream>

namespace DS
{
    template <typename Object>
    class Stack
    {
    public:
        bool empty() const
        { return stack_.empty(); }

        void push(const Object& x)
        { stack_.push_back(x); }

        const Object& top() const
        { return stack_.back(); }

        void pop()
        { stack_.pop_back(); }

    private:
        List<Object> stack_;
    };

}

#endif //__STACK_H__
