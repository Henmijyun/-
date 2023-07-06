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
    server->BindService(debug);
    server->Start();
    return 0;
}