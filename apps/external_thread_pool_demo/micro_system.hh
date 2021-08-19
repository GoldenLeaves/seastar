/**
 * DAIL in Alibaba Group
 *
 */

#pragma once

#include <seastar/core/reactor.hh>

class micro_system {
    boost::optional<seastar::reactor::poller> _my_thread_pool_poller = {};
    static bool _stopped;
public:
    static seastar::future<> configure();
    seastar::future<> stop();
    void exit();
private:
    void register_pollers();
    void deregister_pollers();
};

extern thread_local micro_system *local_micro_engine;

void allocate_micro_engine();

inline micro_system& micro_engine() {
    return *local_micro_engine;
}