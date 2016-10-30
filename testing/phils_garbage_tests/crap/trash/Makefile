# name of your application
APPLICATION = Trash

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += gnrc_pktdump
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_netdev_default
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

#
USEMODULE += gnrc_conn_udp

USEMODULE += shell

USEMODULE += ps

USEMODULE += xtimer

USEMODULE += cpp11-compat

USEPKG += libcoap

# include this for printing IP addresses
USEMODULE += shell_commands

USEMODULE += gnrc_ipv6_default

USEMODULE += gnrc_conn_udp

USEMODULE += posix_sockets


include $(RIOTBASE)/Makefile.include

CFLAGS += -Wno-error
CFLAGS += -w


