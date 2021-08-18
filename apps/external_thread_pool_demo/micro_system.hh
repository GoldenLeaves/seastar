/**
 * DAIL in Alibaba Group
 *
 */

#pragma once

#include <seastar/core/reactor.hh>

class micro_system {
    boost::optional<seastar::reactor::poller> _my_thread_pool_poller = {};
public:
    static seastar::future<> configure();
    void exit();
private:
    void register_pollers();
    void deregister_pollers();
    void clear_micro_system();
};

extern __thread micro_system *local_micro_engine;

void allocate_micro_engine();

inline micro_system& micro_engine() {
    return *local_micro_engine;
}