#ifndef S_VECTOR_H
#define S_VECTOR_H

#include <iostream>
#include <fstream>
#include <stdexcept>

template <typename T>
class s_vector
{
    public:

        s_vector()
        {
            this->size = 0;
            this->capacity = 8;

            try
            {
                this->elements = new T[this->capacity];
            }
            catch(const std::bad_alloc& e)
            {
                throw std::runtime_error("s_vector::ctor()::could_not_allocate_memory\n");              
            }         
        }

        s_vector(T* elements, size_t size, size_t capacity)
        {
            if(!is_valid_size(size))
            {
                throw std::invalid_argument("s_vector::ctor(args)::invalid_size\n");
            }

            if(!is_valid_capacity(capacity))
            {
                throw std::invalid_argument("s_vector::ctor(args)::invalid_capacity\n");
            }

            this->size = size;
            this->capacity = capacity;

            try
            {
                this->elements = new T[this->capacity];
            }
            catch(const std::bad_alloc& e)
            {
                throw std::runtime_error("s_vector::ctor(args)::could_not_allocate_memory\n");
            }

            try
            {
                for (size_t i = 0; i < this->size; ++i)
                {
                    this->elements[i] = elements[i];
                }
            }
            catch(...)
            {
                delete[] this->elements;
                throw std::invalid_argument("s_vector::ctor(args)::invalid_elements\n");
            }      
        }

        s_vector(const s_vector<T>& other)
        {
            copy(other);
        }

        s_vector<T>& operator=(const s_vector<T>& other)
        {
            if(this != &other)
            {
                free();
                copy(other);
            }

            return *this;
        }

        ~s_vector()
        {
            free();
        }

        const size_t get_size() const
        {
            return this->size;
        }

        const size_t get_capacity() const
        {
            return this->capacity;
        }

        const T* get_elements() const
        {
            return this->elements;
        }

        void set_size(const size_t size)
        {
            if(!is_valid_size(size))
            {
                throw std::invalid_argument("s_vector::set_size()::invalid_size\n");
            }

            this->size = size;
        }

        void set_capacity(const size_t capacity)
        {
            if(!is_valid_capacity(capacity))
            {
                throw std::invalid_argument("s_vector::set_capacity()::invalid_capacity\n");
            }

            this->capacity = capacity;
        }

        void set_elements(const T* elements)
        {
            try
            {
                this->elements = new T[this->capacity];
            }
            catch(const std::bad_alloc& e)
            {
                throw std::runtime_error("s_vector::set_elements()::could_not_allocate_memory\n");
            }
            
            try
            {
                for (size_t i = 0; i < this->size; ++i)
                {
                    this->elements[i] = elements[i];
                }          
            }
            catch(...)
            {
                delete[] this->elements;
                throw std::invalid_argument("s_vector::set_elements::invalid_elements\n");
            }
            
        }

        bool push_back(const T& element)
        {
            if(this->size >= this->capacity)
            {
                if(!this->resize())
                {
                    return false;
                }
            }

            this->elements[this->size] = element;
            ++this->size;

            return true;
        }

        bool pop_index(size_t idx)
        {
            if(idx < 0 || idx >= this->size)
            {
                return false;
            }

            for (size_t i = idx; i < this->size - 1; ++i)
            {
                this->elements[i] = this->elements[i + 1];
            }

            --this->size;
            return true;
        }

        const T& operator[](size_t idx) const
        {
            if(idx < 0 || idx >= this->size)
            {
                throw std::out_of_range("s_vector::operator[]\n");
            }

            return this->elements[idx];
        }

    private:

        T* elements;
        size_t size;
        size_t capacity;

        const bool is_valid_size(const size_t size) const
        {
            return size >= 0 && size < this->capacity;
        }

        const bool is_valid_capacity(const size_t capacity) const
        {
            return capacity >= 8;
        }

        bool copy(const s_vector<T>& other)
        {
            this->size = other.size;
            this->capacity = other.capacity;

            try
            {
                this->elements = new T[this->capacity];
            }
            catch(const std::bad_alloc& e)
            {
                return false;
            }
            
            for (size_t i = 0; i < this->size; ++i)
            {
                this->elements[i] = other.elements[i];
            }

            return true;
        }

        void free()
        {
            delete[] this->elements;

            this->elements = nullptr;
            this->size = 0;
            this->capacity = 0;
        }

        bool resize()
        {
            T* buffer = nullptr;

            try
            {
                buffer = new T[this->capacity * 2];
            }
            catch(const std::bad_alloc& e)
            {
                delete[] buffer;
                return false;
            }

            this->capacity *= 2;
         
            for (size_t i = 0; i < this->size; ++i)
            {
                buffer[i] = this->elements[i];
            }

            delete[] this->elements;
            this->elements = buffer; 

            return true;          
        }
};

#endif