#include "ITimer.h"

#include <memory>

#include "TypeAliases.h"

namespace smop
{

    class FakeClock;

    class FakeTimer : public ITimer
    {
    public:
        using ClockPtr = std::shared_ptr<FakeClock>;

        FakeTimer(const ClockPtr& clock, DateTime::duration interval);
        ~FakeTimer();

        void Start() override;
        void Stop() override;
        void SetTimeoutEventHandler(TimeoutHandler timoutEventHandler) override;

        void CheckTimeout();
    
    private:
        const ClockPtr clock_;
        const DateTime::duration interval_;

        DateTime startTime_ = DateTime::min();
        bool isRunning_ = false;
        TimeoutHandler onTimeout_;
    };

}
