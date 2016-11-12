# name of your application
APPLICATION = pHAL_test

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

USEMODULE += shell
# USEMODULE += pwm
USEMODULE += servo
USEMODULE += xtimer

# CFLAGS += -DPWM_NUMOF=4
# CFLAGS += -DPWM_0=1

include $(RIOTBASE)/Makefile.include

# don't treat warnings as errors
CFLAGS += -Wno-error
