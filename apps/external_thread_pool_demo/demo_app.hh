/**
 * DAIL in Alibaba Group
 *
 */

#pragma once

#include <seastar/core/app-template.hh>
#include "micro_system.hh"

class demo_app {
    seastar::app_template _app;
public:
    explicit demo_app(seastar::app_template::config cfg = seastar::app_template::config())
        : _app(std::move(cfg)) {}

    int run(int ac, char **av, std::function<void()> &&func);
};
