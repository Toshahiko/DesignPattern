#pragma once
#include <memory>

#define SMART_PTR_IMPL(x) using x ## _up = std::unique_ptr< x > ; \
                          using x ## _sp = std::shared_ptr< x > ; \
                          using x ## _wp = std::weak_ptr< x > ;
