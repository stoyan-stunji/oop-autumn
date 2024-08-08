#ifndef ALTERNATE_DATA_SOURCE
#define ALTERNATE_DATA_SOURCE

#include "data_source.h"

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

#endif