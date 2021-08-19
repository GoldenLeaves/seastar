/**
 * DAIL in Alibaba Group
 *
 */

#include "demo_app.hh"
#include "my_thread_pool.hh"

int demo_app::run(int ac, char **av, std::function<void()> &&func) {
    return _app.run_deprecated(ac, av, [fn = std::move(func)] () mutable {
        return micro_system::configure().then(
            std::move(fn)
        ).then_wrapped([] (auto&& f) {
            try {
                f.get();
            } catch (...) {
                std::cerr << "Demo-App failed with exception: "
                          << std::current_exception() << std::endl;
                micro_engine().exit();
            }
        });
    });
}

