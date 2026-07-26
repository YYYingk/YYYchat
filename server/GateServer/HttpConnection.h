#pragma once
#include "const.h"

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
	friend class LogicSystem;
	HttpConnection(boost::asio::io_context& ioc);
	void Start();
	tcp::socket& GetSocket() {
		return _socket;
	}
private:
	// 定时监测
	void CheckDeadLine();
	// 应答函数
	void WriteResponse();
	// 处理请求
	void HandleReq();
	void PreParseGetParam();
	tcp::socket _socket;
	// 接收数据的buffer
	beast::flat_buffer _buffer{ 8192 };
	// 接收请求
	http::request<http::dynamic_body> _request;
	// 响应回复
	http::response<http::dynamic_body> _response;
	net::steady_timer deadline_{
		// 定时器放在底层的时间循环,socket绑定了上下文，那么socket就能获取到上下文底层的调度器。
		_socket.get_executor(), std::chrono::seconds(60)
	};
	// 
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;
};

