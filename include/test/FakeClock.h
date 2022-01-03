#include <memory>
#include <unordered_set>

#include "IClock.h"
#include "ITimer.h"

class FakeTimer;

class FakeClock : 
    public std::enable_shared_from_this<FakeClock>, public IClock
{
public:
    FakeClock(DateTime currentTime);

    DateTime Now() const override;

    void PassTime(DateTime::duration duration, int repeatPass = 1);
    std::unique_ptr<ITimer> CreateTimer(DateTime::duration interval);
    void RemoveTimer(FakeTimer* timer);

private:
    DateTime now_;
    std::unordered_set<FakeTimer*> timers_;
};
