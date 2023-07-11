// 测试jsoncpp库的使用

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

int main()
{
    int a = 10;
    int b = 20;
    char c = '+';
    
    // [Key, Val] 的结构
    Json::Value root;   // 万能对象 
    root["aa"] = a;
    root["bb"] = b;
    root["op"] = c;

    Json::Value sub;
    sub["other"] = 200;
    sub["other1"] = "hello";

    root["sub"] = sub;

    // 序列化
    Json::StyledWriter writer;  
    //Json::FastWriter writer;
    std::string s = writer.write(root);   // 转成序列化的字符串风格
    std::cout << s << std::endl;

    // 反序列化
    Json::Reader reader;
    reader.parse(s, root);   // 反序列化写到root中
    a = root["x"].asInt();   // 以int类型，把x的数据写到a
    b = root["y"].asInt();
    c = root["op"].asInt();   // 以 ASCII码传入的，所以转int就行

    return 0;
}