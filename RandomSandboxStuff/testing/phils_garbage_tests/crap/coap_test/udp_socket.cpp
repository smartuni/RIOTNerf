#include "udp_socket.hpp"
#include <cstring>

namespace app {
    udp_socket::udp_socket(std::uint16_t port)
        : m_port(port),
          m_socket(::socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP))
    {
        m_addr.sin6_family = AF_INET6;
        std::memset(&m_addr.sin6_addr, 0, sizeof(m_addr.sin6_addr));
        m_addr.sin6_port = htons(m_port);

        if (m_socket < 0) {
             throw udp_socket_error("failed to create socket in udp_socket ctor");
        }


        if (::bind(m_socket,
                   (struct sockaddr *) &m_addr,
                   sizeof(m_addr)) < 0) {
            throw udp_socket_error("failed to bind socket in udp_socket ctor");
        }

    }

    udp_socket::~udp_socket() {
        ::close(m_socket);
    }

    socklen_t udp_socket::read(buffer_base &write_to) {
        int res;
        struct sockaddr_in6 src;
        socklen_t src_len = sizeof(struct sockaddr_in6);
        if ((res = ::recvfrom(m_socket,
                              write_to.data(),
                              write_to.size(),
                              0,
                              (struct sockaddr *) &src,
                              &src_len)) < 0) {
            throw udp_socket_error("failed to receive data in udp_socket::read");
        } else if (res == 0) {
            throw udp_socket_error("peer shut down error in udp_socket::read");
        }
        return src_len;
    }

    void udp_socket::write(buffer_base &read_from) {
        struct sockaddr_in6 dst;
        dst.sin6_family = AF_INET6;
        dst.sin6_port = htons(m_port);

        if (::sendto(m_socket,
                     read_from.data(),
                     read_from.size(),
                     0,
                     (struct sockaddr *) &dst,
                     sizeof(dst)) < 0) {

            throw udp_socket_error("failed to write data in udp_socket::write");
        }
    }

    udp_socket_error::udp_socket_error(std::string const &what_arg)
        : base_type(what_arg) { }

    udp_socket_error::udp_socket_error(char const *what_arg)
        : base_type(what_arg) { }

    char const *udp_socket_error::what() const noexcept {
        return base_type::what();
    }    
} // END of namespace app
