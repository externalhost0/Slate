//
// Created by Hayden Rivas on 11/10/24.
//

#ifndef SLATE_UUID_H
#define SLATE_UUID_H

#include <functional>

namespace Slate {
    class UUID {
    public:
        UUID();
        explicit UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        explicit operator uint64_t() const { return m_UUID; }

        bool operator==(const UUID& other) const {
            return m_UUID == other.m_UUID;
        }
        bool operator!=(const UUID& other) const {
            return !(*this == other);
        }

    private:
        uint64_t m_UUID;
    };
}
// hash is required for the unordered mapping
namespace std {
    template<> struct hash<Slate::UUID> {
        std::size_t operator()(const Slate::UUID& uuid) const noexcept {
            return static_cast<std::size_t>(static_cast<uint64_t>(uuid));
        }
    };
}



#endif //SLATE_UUID_H
