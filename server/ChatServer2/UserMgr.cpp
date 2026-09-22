#include "UserMgr.h"
#include "CSession.h"
#include "RedisMgr.h"

UserMgr:: ~UserMgr() {
	_uid_to_session.clear();
}


std::shared_ptr<CSession> UserMgr::GetSession(int uid)
{
	// 对Session的操作没有加分布式锁，只加了线程锁，
	// 因为我的思路是在最外层加分布式锁，而接口内部只加线程锁，保证同一个服务器操作的原子性。
	std::lock_guard<std::mutex> lock(_session_mtx);
	auto iter = _uid_to_session.find(uid);
	if (iter == _uid_to_session.end()) {
		return nullptr;
	}

	return iter->second;
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<CSession> session)
{
	std::lock_guard<std::mutex> lock(_session_mtx);
	_uid_to_session[uid] = session;
}

void UserMgr::RmvUserSession(int uid, std::string session_id)
{
	{
		std::lock_guard<std::mutex> lock(_session_mtx);
		auto iter = _uid_to_session.find(uid);
		if (iter != _uid_to_session.end()) {
			return;
		}

		auto session_id_ = iter->second->GetSessionId();
		//不相等说明是其他地方登录了
		if (session_id_ != session_id) {
			return;
		}
		_uid_to_session.erase(uid);
	}
}

UserMgr::UserMgr()
{

}