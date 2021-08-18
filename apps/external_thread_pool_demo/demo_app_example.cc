//
// Created by everlighting on 2020/8/19.
//

#include "demo_app.hh"
#include "my_thread_pool.hh"
#include <seastar/core/reactor.hh>
#include <seastar/core/sleep.hh>

using namespace std::chrono_literals;

int main(int argc, char** argv) {
    demo_app app_example;
    app_example.run(argc, argv, [] {
        return seastar::parallel_for_each(boost::irange<unsigned>(0u, seastar::smp::count), [] (unsigned id) {
            return seastar::smp::submit_to(id, [id] {
                return my_thread_pool::submit_work<int>([id] {
                    seastar::seastar_logger.info("I'm working in my_thread! from micro_engine {} \n", id);
                    return id * id;
                }).then_wrapped([id] (seastar::future<int> fut) {
                    try{
                        seastar::seastar_logger.info("Micro_engine {} got the result {} from my_thread.\n", id, fut.get0());
                    } catch (std::exception& ex) {
                        seastar::seastar_logger.error("Exception: {}\n", ex.what());
                    }
                });
            });
        }).then([] {
            seastar::seastar_logger.info("exit.\n");
            micro_engine().exit();
        });
    });
}