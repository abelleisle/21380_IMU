#pragma once

#include <stdint.h>

template <class T, size_t N>
class static_queue
{
    private:
        T _data[N];
        const size_t _sz = sizeof(T);

        size_t _front;
        size_t _back;
        size_t _size;
        
    public:
        static_queue(void) noexcept
        {
            _size = _front = _back = 0;
        }

        /**
         * Returns the number of elements in the queue
         *
         * @return[size_t] - How many elements are in the queue
         */
        size_t size(void) const noexcept
        {
            return _size;
        }

        /**
         * Returns whether or not the queue is empty
         *
         * @return[bool] - True if empty, False otherwise
         */
        bool empty(void) const noexcept
        {
            return (_size == 0);
        }

        /**
         * Returns maximum size of the queue
         *
         * @return[size_t] - Maximum number of elements in the queue
         */
        constexpr size_t max_size(void) const noexcept
        {
            return N;
        }

        /**
         * Pushes value to the back of the queue.
         * If the queue is full, the front value will be deleted and 'v' will be
         * placed at the back.
         *
         * @param[v] - The element to place at the back
         */
        void push(const T& v)
        {
            if (_size < N) {
            // NOT FULL - Push v to the back of the stack
                _data[_back++] = v;
                _back %= N;
                _size++;
            } else {
            // FULL - Replace front element with v
               _data[_back++] = v;
               _back %= N;
               _front = (_front + 1) % N;
            }
        }

        /**
         * Pops an element off the front of the queue. If the queue is empty
         * nothing will happen.
         */
        void pop(void)
        {
            if (!empty()) {
                _front++;
                _size--;
            }
        }

        /**
         * Gives a reference to the first element of the queue.
         *
         * @return[T&] - First element reference
         */
        T& front(void)
        {
            return _data[_front];
        }

        /**
         * Gives a reference to the last element of the queue.
         *
         * @return[T&] - Last element reference
         */
        T& back(void)
        {
            return _data[_back];
        }
};
