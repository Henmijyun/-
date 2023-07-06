// 计算器的客户端

#include "TcpServer.hpp"
#include "Protocol.hpp"

#include <memory>

using namespace skk_tcpserver;
using namespace skk_protocol;

static void Usage(const std::string& process)
{
    std::cout << "\nUsage: " << process << " port\n" << std::endl; 
}

void debug(int sock)
{
    std::cout << "我是测试服务,得到的sock: " << sock << std::endl;
}

// 计算
static Response calculatorHelp(const Request& req)
{
    Response resp(0, 0);
    switch (req._op)
    {
    case '+':
        resp._result = req._x + req._y;
        break;
    case '-':
        resp._result = req._x - req._y;
        break;
    case '*':
        resp._result = req._x * req._y;
        break;
    case '/':
        if (0 == req._y)
        {
            resp._code = 1;
        }
        else
        {
            resp._result = req._x / req._y;    
        }
        break;
    case '%':
        if (0 == req._y)
        {
            resp._code = 2;
        }
        else
        {
            resp._result = req._x % req._y;    
        }
        break;
    default:
        resp._code = 3;
        break;
    }

    return resp;
}

// 计算器
void calculator(int sock)
{
    while (true)
    {
        std::string str = Recv(sock);  // 读到一个请求
        Request req;
        req.Deserialized(str);   // 反序列化 （从字符串中提取数据）
        Response resp = calculatorHelp(req);  // 计算
        std::string respString = resp.Serialize();  // 对计算结果进行序列化
        Send(sock, respString);  // 发送数据
    }
}

// ./CalServer port
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::unique_ptr<TcpServer> server(new TcpServer(atoi(argv[1])));
    server->BindService(calculator);
    server->Start();


    // Request req(10, 5, '+');
    // std::string s = req.Serialize();  // 序列化
    // std::cout << s <<std::endl;

    // Request temp;
    // temp.Deserialized(s);   // 反序列化
    // std::cout << temp._x << std::endl;
    // std::cout << temp._op << std::endl;
    // std::cout << temp._y << std::endl;


    return 0;
}