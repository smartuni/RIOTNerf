#ifndef APP_UDP_SOCKET_HPP
#define APP_UDP_SOCKET_HPP
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include "buffer.hpp"
#include "socket.h"

namespace app
{
    class udp_socket final {
    public:
        using this_type = udp_socket;

        udp_socket(this_type const &) = delete;
        this_type &operator=(this_type const &) = delete;

        explicit udp_socket(std::uint16_t port);

        ~udp_socket();

        socklen_t read(buffer_base &write_to);

        void write(buffer_base &read_from);

    private:
        std::uint16_t m_port;
        int m_socket;
        struct sockaddr_in6 m_addr;
    }; // END of class udp_socket

    class udp_socket_error
        : public std::runtime_error
    {
    public:
        using this_type = udp_socket_error;
        using base_type = std::runtime_error;

        explicit udp_socket_error(std::string const &what_arg);
        explicit udp_socket_error(char const *what_arg);

        virtual char const *what() const noexcept override;
    }; // END of class udp_socket_error

} // END of namespace app

#endif /* APP_UDP_SOCKET_HPP */
