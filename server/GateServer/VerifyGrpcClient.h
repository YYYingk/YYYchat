#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port);
	~RPConPool();

	void Close();
	std::unique_ptr<VarifyService::Stub> getConnection();

	void returnConnection(std::unique_ptr<VarifyService::Stub> context);
private:
	atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VarifyService::Stub>> connections_;
	std::condition_variable cond_;
	std::mutex mutex_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVarifyRsp GetVerifyCode(std::string email);
private:
	VerifyGrpcClient();
	std::unique_ptr<RPConPool> pool_;
};

