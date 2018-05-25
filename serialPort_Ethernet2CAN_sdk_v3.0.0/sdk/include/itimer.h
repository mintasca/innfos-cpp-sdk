#ifndef ITIMER_H
#define ITIMER_H
#include "asio/steady_timer.hpp"
#include "v8stdint.h"

#include <functional>

namespace Actuator {
using timeoutCallback = std::function<void()>;
class ITimer
{
public:
    static void singleShot(uint32_t interval,timeoutCallback callback,asio::io_context & ioContext);
    ITimer(uint32_t interval,timeoutCallback callback,asio::io_context & ioContext,bool bSingle = false);
    ~ITimer();
    void setInterval(uint32_t interval);
    void stop();
    void start(uint32_t interval=0);
protected:
    void waitHandler(const asio::error_code& ec);
private:
    timeoutCallback _callback;
    uint32_t m_nInterval;
    asio::steady_timer * m_pTimer;
    asio::io_context * m_pIOContext;
    bool m_bSingleShoot;
};

}


#endif // ITIMER_H
