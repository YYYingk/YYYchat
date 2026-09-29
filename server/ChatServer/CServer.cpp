#include "CServer.h"
#include <iostream>
#include "AsioIOServicePool.h"
#include "UserMgr.h"
#include "RedisMgr.h"
#include "ConfigMgr.h"

CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(), port)), _timer(io_context, std::chrono::seconds(60))
{
	cout << "Server start success, listen on port : " << _port << endl;
	_timer.async_wait([this](boost::system::error_code ec) {
		on_timer(ec);
		});
	StartAccept();
}

CServer::~CServer() {
	cout << "Server destruct listen on port : " << _port << endl;
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Start();
		lock_guard<mutex> lock(_mutex);
		_sessions.insert(make_pair(new_session->GetSessionId(), new_session));
	}
	else {
		cout << "session accept failed, error is " << error.what() << endl;
	}

	StartAccept();
}

void CServer::StartAccept() {
	auto &io_context = AsioIOServicePool::GetInstance()->GetIOService();
	shared_ptr<CSession> new_session = make_shared<CSession>(io_context, this);
	// 异步等连接来然后回调
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}

void CServer::ClearSession(std::string session_id) {
	lock_guard<mutex> lock(_mutex);
	if (_sessions.find(session_id) != _sessions.end()) {
		auto uid = _sessions[session_id]->GetUserId();

		//移除用户和session的关联
		UserMgr::GetInstance()->RmvUserSession(uid, session_id);
	}

	_sessions.erase(session_id);
}

bool CServer::CheckValid(std::string uuid)
{
	lock_guard<mutex> lock(_mutex);
	auto it = _sessions.find(uuid);
	if (it != _sessions.end()) {
		return true;
	}
	return false;
}

//根据用户获取session
shared_ptr<CSession> CServer::GetSession(std::string uuid) {
	lock_guard<mutex> lock(_mutex);
	auto it = _sessions.find(uuid);
	if (it != _sessions.end()) {
		return it->second;
	}
	return nullptr;
}

void CServer::on_timer(const boost::system::error_code& ec)
{
	std::vector<std::shared_ptr<CSession>> _expired_sessions;
	int session_count = 0;
	{
		lock_guard<mutex> lock(_mutex);
		time_t now = std::time(nullptr);
		for (auto it = _sessions.begin(); it != _sessions.end(); ++it) {
			auto session = it->second;
			auto b_expired = session->IsHeartbeatExpire(now);
			if (b_expired){
				session->Close();
				_expired_sessions.push_back(session);
				continue;
			}
			else {
				session_count++;
			}
		}
	}

	// 设置session数量到redis
	auto& cfg = ConfigMgr::Inst();
	auto self_name = cfg.GetValue("server", "self_name");
	auto count_str = std::to_string(session_count);
	RedisMgr::GetInstance()->HSet(LOGIN_COUNT, self_name, count_str);

	// 处理过期的session
	for(auto& session : _expired_sessions) {
		session->DealException();
	}

	// 再设置下一个60s检测
	_timer.expires_after(std::chrono::seconds(60));
	_timer.async_wait([this](boost::system::error_code ec) {
		on_timer(ec);
		});
}