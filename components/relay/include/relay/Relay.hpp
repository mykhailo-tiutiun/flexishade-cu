#ifndef RELAY_HPP
#define RELAY_HPP

#include "soc/gpio_num.h"
#include <optional>
#include <string>
#include <variant>

struct LocalId {
    int val;

    LocalId() : val(0) {}
    explicit LocalId(int v) : val(v) {}

    bool operator<(const LocalId& other) const {
        return val < other.val;
    }
};

struct GlobalId {
    std::string val;

    GlobalId() = default;
    explicit GlobalId(std::string v) : val(std::move(v)) {}

    bool operator<(const GlobalId& other) const {
        return val < other.val;
    }
};

using RelayId = std::variant<LocalId, GlobalId>;

class Relay {
    private:
        LocalId local_id_;
        gpio_num_t gpio_num_;
        std::optional<GlobalId> global_id_;
        bool is_open_;

    public:
        Relay(LocalId local_id, int gpio_num);
        void open();
        void close();
        void toggle();

        const LocalId& getLocalId() const;
        const std::optional<GlobalId>& getGlobalId() const;

        bool hasGlobalId() const;
        bool isOpen() const;

        void setGlobalId(GlobalId&& global_id_);
};

#endif
