#ifndef SOURCES_H
#define SOURCES_H

#include "s_vector.h"

#include <cstring>

template <typename T>
class data_source
{
    public:
    
        virtual ~data_source() {}
        virtual data_source<T>* clone() const = 0;

        virtual T next() = 0;

        virtual s_vector<T> next_n(const size_t n)
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

template <typename T>
class file_data_source : public data_source<T> 
{
    public:

        virtual data_source<T>* clone() const
        {
            return new file_data_source<T>(this->file_name.c_str());
        }

        file_data_source(const char* fn) : file(fn), file_name(fn)
        {
            if (!file.is_open())
            {
                throw std::runtime_error("file_data_source::ctor::cannot_open_file\n");
            }
        }

        virtual T next()
        {
            if(this->end())
            {
                throw std::out_of_range("file_data_source::next()\n");
            }

            T element;
            this->file >> element;
            return element;
        }

        virtual bool reset()
        {
            if(this->file.fail())
            {
                throw std::runtime_error("file_data_source::next()::file_fail\n");
            }
            
            this->file.clear();
            this->file.seekg(0, std::ios::beg);

            return true;
        }

        virtual bool end() const
        {
            return this->file.eof();
        }

    private:

        std::ifstream file;
        std::string file_name;
};

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

template <typename T>
class alternate_data_source : public data_source<T> 
{
    public:

        virtual data_source<T>* clone() const
        {
            return new alternate_data_source<T>(*this);
        }

        explicit alternate_data_source(const s_vector<data_source<T>*>& ss)
        : sources(ss), idx(0) {}

        virtual T next()
        {
            if(this->end())
            {
                throw std::out_of_range("alternate_data_source::next()\n");
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

        virtual bool reset()
        {
            try
            {
                size_t size = this->sources.get_size();
                for(size_t i = 0; i < size; ++i)
                {
                    this->sources[i]->reset();
                }
                this->idx = 0;
            }
            catch(const std::exception& e)
            {
                std::cout << "alternate_data_source::reset()::error_while_resetting_element\n";
                return false;
            }

            return true;         
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

            const s_vector<data_source<T>*> sources;
            size_t idx;
};

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