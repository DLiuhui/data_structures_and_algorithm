//
// Created by DDRHb on 2019/8/31.
//

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "List.h"
#include <cstdlib>
#include <iostream>

namespace DS
{
    template <typename Object>
    class Queue
    {
    public:
        bool empty() const
        { return queue_.empty(); }

        const Object& front() const
        { return queue_.front(); }

        const Object& back() const
        { return queue_.back(); }

        void enQueue(const Object& x)
        { queue_.push_back(x); }

        void deQueue()
        { queue_.pop_front(); }

    private:
        List<Object> queue_;
    };
}

#endif //__QUEUE_H__
