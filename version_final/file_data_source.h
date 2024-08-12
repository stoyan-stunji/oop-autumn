#ifndef FILE_DATA_SOURCE
#define FILE_DATA_SOURCE

#include "data_source.h"

template <typename T>
class file_data_source : public data_source<T> 
{
    public:

        virtual data_source<T>* clone() const
        {
            return new file_data_source<T>(this->file_name.c_str());
        }

        explicit file_data_source(const char* fn) : file(fn), file_name(fn)
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
            if(!this->file)
            {
                throw std::runtime_error("file_data_source::next()::file_error\n");
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


#endif