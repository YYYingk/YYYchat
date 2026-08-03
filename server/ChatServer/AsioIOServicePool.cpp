#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_works(size), _nextIOService(0) {
	// 绑定work，以防ioservice因没有任务直接退出
	for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::make_unique<Work>(boost::asio::make_work_guard(_ioServices[i]));
	}

	//遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
		});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
	// 仅仅执行work.reset并不能让io_context从run的状态中退出
	// 当io_context已经绑定了读或写的监听事件后，还需要手动stop该服务。
	for (std::size_t i = 0; i < _works.size(); ++i) {
		_ioServices[i].stop(); // 正确地停止io_context
		_works[i].reset();
	}

	for (auto& t : _threads) {
		t.join();
	}
}

