#pragma once 

namespace bit
{
    class string
    {
    public:
        string(const char* str)
            :_str()
        {

        }

    private:
        char* _str;
        size_t _size;
        size_t _capacity;
    }
}

















