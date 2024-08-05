#ifndef S_VECTOR_H
#define S_VECTOR_H

template <typename T>
class s_vector
{
    public:

        s_vector()
        {
            this->size = 0;
            this->capacity = 8;
            this->elements = new T[this->capacity];
        }

        s_vector(const T* elements, const size_t size, const size_t capacity)
        {
            if(!is_valid_size(size))
            {
                std::cout << "s_vector::ctor::invalid_size\n";
                return;
            }

            if(!is_valid_capacity(capacity))
            {
                std::cout << "s_vector::ctor::invalid_capacity\n";
                return;
            }

            this->size = size;
            this->capacity = capacity;
            this->elements = new (std::nothrow) T[this->capacity];
            for (size_t i = 0; i < this->size; ++i)
            {
                this->elements[i] = elements[i];
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
                std::cout << "s_vector::set_size::invalid_size\n";
                return;
            }

            this->size = size;
        }

        void set_capacity(const size_t capacity)
        {
            if(!is_valid_capacity(capacity))
            {
                std::cout << "s_vector::set_capacity::invalid_capacity\n";
                return;
            }

            this->capacity = capacity;
        }

        void set_elements(const T* elements)
        {
            this->elements = new (std::nothrow) T[this->capacity];
            for (size_t i = 0; i < this->size; ++i)
            {
                this->elements[i] = elements[i];
            }          
        }

        void push_back(const T& element)
        {
            if(this->size >= this->capacity)
            {
                resize();
            }

            this->elements[this->size++] = element;
        }

        void pop_index(const size_t idx)
        {
            if(idx < 0 || idx >= this->size)
            {
                std::cout << "s_vector::pop_index()::cannot_pop_element\n";
                return;
            }

            for (size_t i = idx; i < this->size - 1; ++i)
            {
                this->elements[i] = this->elements[i + 1];
            }

            --this->size;
        }

        T operator[](const size_t idx) const
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
            return size >= 0;
        }

        const bool is_valid_capacity(const size_t capacity) const
        {
            return capacity >= 8;
        }

        void copy(const s_vector<T>& other)
        {
            this->size = other.size;
            this->capacity = other.capacity;
            this->elements = new (std::nothrow) T[this->capacity];
            for (size_t i = 0; i < this->size; ++i)
            {
                this->elements[i] = other.elements[i];
            }
        }

        void free()
        {
            delete[] this->elements;

            this->elements = nullptr;
            this->size = 0;
            this->capacity = 0;
        }

        void resize()
        {
            this->capacity *= 2;
            T* buffer = new (std::nothrow) T[this->capacity];

            for (size_t i = 0; i < this->size; ++i)
            {
                buffer[i] = this->elements[i];
            }

            delete[] this->elements;
            this->elements = buffer;           
        }
};

#endif