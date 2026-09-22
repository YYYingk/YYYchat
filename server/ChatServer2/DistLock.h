#pragma once
#include <string>
#include <hiredis.h>
#include "Singleton.h"

class DistLock : public Singleton<DistLock>
{
    friend class Singleton<DistLock>;
public:
    ~DistLock();
    std::string acquireLock(redisContext* context, const std::string& lockName,
        int lockTimeout, int acquireTimeout);

    bool releaseLock(redisContext* context, const std::string& lockName,
        const std::string& identifier);
private:
    DistLock();
};