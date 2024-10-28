#include "TcpServer.hpp"

#include <memory>

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


// 上层的业务处理   (也可以解耦，push到任务队列，任务处理交给后端线程)
void NetCal(Connection *conn, std::string &request)
{
    // 走到这，一定是一个完整的报文!
    logMessage(DEBUG, "NetCal been called, get request: %s", request.c_str());
    // 1. 反序列化
    Request req;
    if (!req.Deserialized(request))
    {
        return;  // 反序列化失败
    }

    // 2. 业务处理
    Response resp = calculatorHelp(req);

    // 3. 序列化, 构建应答
    std::string send_str = resp.Serialize();
    send_str = Encode(send_str);

    // 4. 交给服务器conn
    conn->_outbuffer += send_str;

    // 5. 让底层的TcpServer，让它进行发送 (重要)
    // 5.1 完整的发送逻辑
    // 5.2 触发发送动作，一旦开启EPOLLOUT，epoll会自动触发一次发送事件，如果后续保持发送的开启，epoll会一直发送
    conn->_tsvr->EnableReadWrite(conn, true, true);   // 打开epoll，自动读写
}

int main()
{
    std::unique_ptr<TcpServer> svr(new TcpServer());
    svr->Dispather(NetCal); // 任务派发
    return 0;
}