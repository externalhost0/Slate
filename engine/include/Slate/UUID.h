//
// Created by Hayden Rivas on 11/10/24.
//

#pragma once

#include <cstdint>
#include <functional>

namespace Slate {
    class UUID {
    public:
        UUID();
        explicit UUID(uint32_t uuid);
        UUID(const UUID&) = default;

        explicit operator uint32_t() const { return m_UUID; }

        bool operator==(const UUID& other) const {
            return m_UUID == other.m_UUID;
        }
        bool operator!=(const UUID& other) const {
            return !(*this == other);
        }

    private:
        uint32_t m_UUID;
    };
}
// hash is required for the unordered mapping
namespace std {
    template<> struct hash<Slate::UUID> {
        size_t operator()(const Slate::UUID& uuid) const noexcept {
            return static_cast<std::size_t>(static_cast<uint32_t>(uuid));
        }
    };
}