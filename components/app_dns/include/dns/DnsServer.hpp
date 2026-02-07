#ifndef DNS_SERVER_HPP
#define DNS_SERVER_HPP

#include <expected>
#include <string>
#include <thread>

class DnsServer
{
    public:
        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        bool isUp() const;

    private:
        bool is_up_;
        int socket_;
        std::optional<std::thread> thread_;

        void serverTask();
};

#endif
