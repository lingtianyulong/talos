#include "lifetime.h"

namespace talos::base {
namespace lifetime {

Lifetime::Lifetime() : _lt(lifetime_new()) {}

Lifetime::~Lifetime() {
    if (_lt) {
        lifetime_destroy(_lt);
        lifetime_free(_lt);
        _lt = nullptr;
    }
}

Lifetime::Lifetime(Lifetime&& that) noexcept : _lt(std::exchange(that._lt, nullptr)) {}

Lifetime& Lifetime::operator=(Lifetime&& that) noexcept {
    if (this != &that) {
        if (_lt) {
            lifetime_destroy(_lt);
            lifetime_free(_lt);
        }
        _lt = std::exchange(that._lt, nullptr);
    }
    return *this;
}

void Lifetime::destroy() {
    if (_lt) {
        lifetime_destroy(_lt);
    }
}

}  // namespace lifetime
}  // namespace talos::base
