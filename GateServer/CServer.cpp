#include "CServer.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port) : _ioc(ioc),
	_acceptor(ioc, tcp::endpoint(tcp::v4(), port)), _socket(ioc)
{

}

void CServer::Start()
{

}
