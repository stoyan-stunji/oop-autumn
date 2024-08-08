#ifndef GENERATOR_DATA_SOURCE
#define GENERATOR_DATA_SOURCE

#include "data_source.h"

template <typename T>
class generator_data_source : public data_source<T>
{
    public:

        virtual data_source<T>* clone() const
        {
            return new generator_data_source<T>(*this);
        }

        explicit generator_data_source(T (*funct)())
        {
            this->functor = funct;
        }

        virtual T next()
        {
            return (*functor)();
        }

        virtual bool reset()
        {
            std::cout << "generator_data_source::reset()::unsupported_action\n";
            return false;

            // by design;
        }

        virtual bool end() const
        {
            return false;
        }

    private:

        T (*functor)();
};


#endif