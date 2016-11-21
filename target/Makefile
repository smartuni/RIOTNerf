APPLICATION = driver_isl29125

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

include $(RIOTBASE)/tests/Makefile.tests_common

FEATURES_REQUIRED = periph_i2c

USEMODULE += isl29125
USEMODULE += xtimer

# set default device parameters in case they are undefined
TEST_ISL29125_I2C     ?= I2C_0
TEST_ISL29125_IRQ_PIN ?= GPIO_PIN\(0,0\)

# export parameters
CFLAGS += -DTEST_ISL29125_I2C=$(TEST_ISL29125_I2C)
CFLAGS += -DTEST_ISL29125_IRQ_PIN=$(TEST_ISL29125_IRQ_PIN)

include $(RIOTBASE)/Makefile.include
