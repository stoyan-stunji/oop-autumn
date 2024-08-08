#ifndef DEFAULT_DATA_SOURCE
#define DEFAULT_DATA_SOURCE

#include "data_source.h"

template <typename T>
class default_data_source : public data_source<T> 
{
    public:

        virtual data_source<T>* clone() const
        {
            return new default_data_source<T>(*this);
        }

        virtual T next() 
        {
            return T();
        }

        virtual bool reset()
        {
            return true;

            // by design;
        }

        virtual bool end() const
        {
            return false;
        }
};

#endif