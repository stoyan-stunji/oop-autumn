#ifndef SOURCES_H
#define SOURCES_H

#include "pre_sources.h"

template <typename T>
class data_source
{
    public:

        explicit data_source(basic_source<T>* bs) : source(bs) {}

        virtual T next()
        {
            T element;

            while (!this->source->end())
            {
                element = this->source->next();
                bool is_removed = false;
                
                for (size_t i = 0; i < this->removed.get_size(); ++i)
                {
                    if (element == this->removed[i])
                    {
                        is_removed = true;
                        break;
                    }
                }

                if (!is_removed)
                {
                    break;
                }
            }

            return element;
        }

        virtual T remove()
        {
            T element = this->source->next();
            this->removed.push_back(element);
            return element;
        }

        virtual s_vector<T> next(const size_t n)
        {
            if (n < 1)
            {
                std::cout << "data_source::next(n)::n_must_be_greater_than_0\n";
                s_vector<T> empty;
                return empty;

                // instead of throwing an exception and "breaking" the whole program;
            }

            s_vector<T> temp;
            size_t k = n;

            while(k > 0)
            {
                if(end())
                {
                    break;
                }
                
                temp.push_back(this->source->next());
                --k;
            }

            return temp;
        }

        virtual bool reset()
        {
            try
            {
                this->source->reset();
            }
            catch(const std::exception& e)
            {
                std::cout << e.what();
                return false;
            }
            
            return true;
        }

        virtual bool end() const
        {
            return this->source->end();
        }

        operator bool() const
        {
            return end();
        }

        T operator()()
        {
            return next();
        }

        friend data_source<T>& operator>>(data_source<T>& ds, T& element)
        {
            element = ds.next();
            return ds;
        }

        virtual basic_source<T>* get_ptr() const
        {
            return this->source;
        }

    private:

        basic_source<T>* source;
        s_vector<T> removed;
};

template <typename T>
class default_data_source : public data_source<T> 
{
    public:

        explicit default_data_source() : data_source<T>(&this->ds) {}
    
    private:

        default_source<T> ds;
};

template <typename T>
class file_data_source : public data_source<T> 
{
    public:

        explicit file_data_source(const char* file_name) 
            : data_source<T>(&this->fs), fs(file_name) {}

    private:

        file_source<T> fs;
};

template <typename T>
class array_data_source : public data_source<T>
{
    public:

        explicit array_data_source(const s_vector<T>& array) 
        : as(array), data_source<T>(&this->as) {}

        array_data_source<T> operator+(const T& element)
        {
            array_data_source<T> copy = *this;
            copy.as += element;
            return copy;
        }

        array_data_source<T>& operator+=(const T& element)
        {
            *this = *this + element;
            return *this;
        }

        void operator--()
        {
            try
            {
                this->as--; 
            }
            catch(const std::exception& e)
            {
                std::cout << "array_data_source::operator--()::cannot_move_index_back\n";
                return;
            }         
        }

        void operator--(int)
        {
            --this->as;
        }

    private:
    
        array_source<T> as;
};

template <typename T>
class alternate_data_source : public data_source<T> 
{
    public:

    explicit alternate_data_source(const s_vector<data_source<T>*>& sources) 
        : as(convert_to_basic_sources(sources)), data_source<T>(&this->as) {}
          
    private:

        alternate_source<T> as;

        s_vector<basic_source<T>*> convert_to_basic_sources(const s_vector<data_source<T>*>& sources)
        {
            s_vector<basic_source<T>*> vector;
            for (size_t i = 0; i < sources.get_size(); ++i)
            {
                vector.push_back(sources[i]->get_ptr());
            }
            return vector;
        }
};

template <typename T>
class generator_data_source : public data_source<T> 
{
    public:

        explicit generator_data_source(T (*functor)()) 
            : fs(functor), data_source<T>(&this->fs) {}
            
    private:

        functor_source<T> fs;
};

#endif
