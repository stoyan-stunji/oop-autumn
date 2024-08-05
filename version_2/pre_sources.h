#ifndef PRE_SOURCES_H
#define PRE_SOURCES_H

#include <iostream>
#include <fstream>
#include <exception>

#include <vector>
#include "s_vector.h"

template <typename T>
class basic_source
{
    public:
    
        virtual ~basic_source() {}

        virtual T next() = 0;
        virtual void reset() = 0;
        virtual bool end() const = 0;

};

template <typename T>
class default_source : public basic_source<T>
{
    public:
    
        virtual T next() 
        {
            if(end())
            {
                throw std::out_of_range("default_source::next()\n");
            }

            return T();
        }

        virtual void reset()
        {
            // empty by design;
        }

        virtual bool end() const
        {
            return false;
        }
};

template <typename T>
class file_source : public basic_source<T>
{
    public:

        explicit file_source(const char* file_name) : file(file_name)
        {
            if (!file.is_open())
            {
                throw std::runtime_error("file_source::cannot_open_file\n");
            }
        }

        virtual T next()
        {
            if(end())
            {
                throw std::out_of_range("file_source::next()\n");
            }

            T element;
            this->file >> element;
            return element;
        }

        virtual void reset()
        {
            if(this->file.fail())
            {
                throw std::runtime_error("file_source::next()::file_fail\n");
            }
            
            this->file.clear();
            this->file.seekg(0, std::ios::beg);
        }

        virtual bool end() const
        {
            return this->file.eof();
        }

    private:

        std::ifstream file;
};

template <typename T>
class array_source : public basic_source<T>
{
    public:
        
        explicit array_source(const s_vector<T>& arr) : array(arr), idx(0) {}

        virtual T next()
        {
            if (end())
            {
                throw std::out_of_range("array_source::next()\n");
            }

            return this->array[idx++];
        }

        virtual void reset()
        {
            this->idx = 0;
        }

        virtual bool end() const
        {
            return this->idx == this->array.get_size();
        }

        array_source<T> operator+(const T& element)
        {
            array_source<T> copy = *this;
            copy.array.push_back(element);
            return copy;
        }

        array_source<T>& operator+=(const T& element)
        {
            *this = *this + element;
            return *this;
        }

        void operator--()
        {
            if (!idx)
            {
                throw std::out_of_range("array_source::operator--\n");
            }
            --idx;
        }

        void operator--(int)
        {
            --(*this);
        }

    private:

        s_vector<T> array;
        size_t idx;
};

template <typename T>
struct alternate_source : basic_source<T>
{
    explicit alternate_source(const s_vector<basic_source<T>*>& ss)
    : sources(ss), idx(0) {}

    virtual T next()
    {
        if(end())
        {
            throw std::out_of_range("alternate_source::next()\n");
        }

        do
        {
            if (this->idx >= this->sources.get_size())
            {
                this->idx = 0;
            }

            if (!this->sources[this->idx]->end())
            {
                break;
            }
            else
            {
                ++this->idx;
            }
        } while (true);
            
        return this->sources[this->idx++]->next();
    }

    virtual void reset()
    {
        size_t size = this->sources.get_size();
        for(size_t i = 0; i < size; ++i)
        {
            this->sources[i]->reset();
        }
        this->idx = 0;
    }

    virtual bool end() const
    {
        bool flag = true;
        size_t size = this->sources.get_size();
        for(size_t i = 0; i < size; ++i)
        {
            if(!this->sources[i]->end())
            {
                flag = false;
            }
        }
        
        return flag;
    }

    private:

        const s_vector<basic_source<T>*> sources;
        size_t idx;
};

template <typename T>
class functor_source : public basic_source<T>
{
    public:

        explicit functor_source(T (*funct)())
        {
            this->functor = funct;
        }

        virtual T next()
        {
            return (*functor)();
        }

        virtual void reset()
        {
            throw std::runtime_error("functor_source::reset()::unsupported_action\n");

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