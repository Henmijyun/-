// 切割子串

#pragma once

#include <iostream>
#include <vector>

class Util
{
public:
    // 切割子串    aaa\r\nbbb\r\nccc\r\n
    static void cutString(const std::string& s, const std::string& sep, std::vector<std::string>* out)
    {
        std::size_t start = 0;
        while(start < s.size())
        {
            auto pos = s.find(sep, start);
            if (pos == std::string::npos)
            {
                break;
            }
            // 截取
            std::string sub = s.substr(start, pos - start);
            // std::cout << "---" << sub << std::endl;
            out->push_back(sub);
            start += sub.size();
            start += sep.size();
        }
        if (start < s.size())
        {
            out ->push_back(s.substr(start));  // 还有数据的话，全部取出来
        }
    }
};