#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

#include "s_vector.h"

#include <cstring>

template <typename T>
class data_source
{
    public:
    
        virtual ~data_source() {}
        virtual data_source<T>* clone() const = 0;

        virtual T next() = 0;

        virtual s_vector<T> next_n(size_t n)
        {
            if(n < 1)
            {
                throw std::out_of_range("data_source::next_n()::n_must_be_greater_than_1\n");
            }

            s_vector<T> temp;
            size_t k = n;

            while(k > 0)
            {
                if(this->end())
                {
                    break;
                }
                
                temp.push_back(this->next());
                --k;
            }

            return temp;
        }

        virtual bool end() const = 0;
        virtual bool reset() = 0;

        T operator()()
        {
            return this->next();
        }

        operator bool() const
        {
            return end();
        }

        friend data_source<T>& operator>>(data_source<T>& ds, T& element)
        {
            element = ds.next();
            return ds;
        }
};

#endif