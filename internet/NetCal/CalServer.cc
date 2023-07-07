// 计算器的客户端

#include "TcpServer.hpp"
#include "Protocol.hpp"

#include <signal.h>

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
    std::string inbuffer;
    while (true)
    {
        // 1.读取数据
        bool res = Recv(sock, &inbuffer);  // 读取成功
        if (!res)
        {
            break;
        }

        // 2.协议解析，保证得到一个完整的报文
        std::string package = Decode(inbuffer);   // 得到完整报文
        if (package.empty())
        {
            // 如果是空串，循环继续读数据
            continue;
        }

        Request req;
        req.Deserialized(package);   // 反序列化 （从字符串中提取数据）
        Response resp = calculatorHelp(req);  // 计算
        std::string respString = resp.Serialize();  // 对计算结果进行序列化

        // "length\r\n_code _result\r\n"
        Send(sock, respString);  // 发送数据
    }
}

// void handler(int signo)
// {
//     std::cout << "get a signo: " << signo << std::endl;
//     exit(0);
// }

// ./CalServer port
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    // signal(SIGPIPE, handler);
    
    // 一般：server在编写的时候，要有较严谨的判断逻辑
    // 一般服务器都是要忽略SIGPIPE信号的，防止在运行中出现非法写入的问题！
    signal(SIGPIPE, SIG_IGN);  // SIG_IGN忽略该信号

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