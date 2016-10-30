#include "udp_socket.hpp"
#include "buffer.hpp"
#include <string> 
#include <cstdio>
#include <cstring>

int main() {
    try {
        app::udp_socket s{ 1234 };
        //app::udp_socket s2{ 1234 };

        app::buffer<50> buf{ };

        std::string string("i like cookies");
        std::memcpy(buf.data(), string.data(), string.size());
        //s.write(buf);

        //app::buffer<50> buf2{ };
        //s2.read(buf2);

       // std::printf("buf2: %s\n", buf2.data());
     } catch (app::udp_socket_error const &e) {
         std::fprintf(stderr, "err: %s\n", e.what());
     }
}
