#include <nova/logger/logger.hpp>
#include <thread>

int main() {
    Nova::Logger l("test");
    Nova::Logger _l("tester");
    NOVA_LOW_LOG_INFO(_l, "test");
    NOVA_LOW_LOG_INFO(_l, "test");
    NOVA_LOW_LOG_INFO(_l, "test");
    NOVA_LOW_LOG_INFO(_l, "test");
    NOVA_LOW_LOG_INFO(_l, "test");
    return 0;
}