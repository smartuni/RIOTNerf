export APPLICATION = driver_servo
include ../RIOT/tests/Makefile.tests_common

FEATURES_REQUIRED = periph_pwm

USEMODULE += xtimer
USEMODULE += servo

# This has to be the absolute path to the RIOT base directory:
 RIOTBASE ?= $(CURDIR)/../RIOT

include $(RIOTBASE)/Makefile.include
