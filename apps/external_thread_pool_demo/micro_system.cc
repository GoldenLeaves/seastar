/**
 * DAIL in Alibaba Group
 *
 */

#include "micro_system.hh"
#include "my_thread_pool.hh"

bool micro_system::_stopped = false;

void micro_system::register_pollers() {
    _my_thread_pool_poller = seastar::reactor::poller(std::make_unique<my_thread_pool_pollfn>());
}

void micro_system::deregister_pollers() {
    _my_thread_pool_poller = {};
}

seastar::future<> micro_system::stop() {
    return seastar::smp::invoke_on_all([] {
        micro_engine().deregister_pollers();
    }).then([] {
        my_thread_pool::stop();
        micro_system::_stopped = true;
    });
}

seastar::future<> micro_system::configure() {
    my_thread_pool::configure();
    return seastar::smp::invoke_on_all([] {
        allocate_micro_engine();
        micro_engine().register_pollers();
    });
}

void micro_system::exit() {
    assert(_stopped);
    seastar::engine().exit(0);
}

struct mirco_engine_deleter {
    void operator()(micro_system* ms) {
        ms->~micro_system();
        free(ms);
    }
};

thread_local micro_system* local_micro_engine{nullptr};
thread_local std::unique_ptr<micro_system, mirco_engine_deleter> mirco_engine_holder;

void allocate_micro_engine() {
    assert(!mirco_engine_holder);
    void *buf;
    int r = posix_memalign(&buf, seastar::cache_line_size, sizeof(micro_system));
    assert(r == 0);
    local_micro_engine = reinterpret_cast<micro_system*>(buf);
    new (buf) micro_system();
    mirco_engine_holder.reset(local_micro_engine);
}
