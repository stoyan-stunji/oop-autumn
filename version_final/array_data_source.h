#ifndef ARRAY_DATA_SOURCE
#define ARRAY_DATA_SOURCE

#include "data_source.h"

template <typename T>
class array_data_source : public data_source<T>
{
    public:

        virtual data_source<T>* clone() const
        {
            return new array_data_source<T>(*this);
        }

        explicit array_data_source(const s_vector<T>& arr) 
        : array(arr), idx(0) {}

        virtual T next()
        {
            if (this->end())
            {
                throw std::out_of_range("array_data_source::next()\n");
            }

            return this->array[idx++];
        }

        virtual bool reset()
        {
            this->idx = 0;
            return true;
        }

        virtual bool end() const
        {
            return this->idx == this->array.get_size();
        }

        array_data_source<T> operator+(const T& element)
        {
            array_data_source<T> copy = *this;
            copy.array.push_back(element);
            return copy;
        }

        array_data_source<T>& operator+=(const T& element)
        {
            *this = *this + element;
            return *this;
        }

        void operator--()
        {
            if (!this->idx)
            {
                throw std::out_of_range("array_data_source::operator--\n");
            }
            --this->idx;
        }

        void operator--(int)
        {
            --(*this);
        }

    private:

        s_vector<T> array;
        size_t idx;
};

#endif