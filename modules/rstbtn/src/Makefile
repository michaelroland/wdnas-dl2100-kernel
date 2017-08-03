################################################################################
#
# Reset Button Kernel Module for WD DL2100 NAS
# 
# Copyright (c) 2017 Michael Roland <mi.roland@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
################################################################################

###########################################################################
DRIVER_CFILES = 
DRIVER_CFLAGS = 
DRIVER_NAME = rstbtn
###########################################################################

ifneq ($(KERNELRELEASE),)
# Kbuild part of Makefile
obj-m := $(DRIVER_NAME).o
$(DRIVER_NAME)-y := $(DRIVER_CFILES:.c=.o)
ccflags-y := $(DRIVER_CFLAGS)
else
# Regular Makefile

ifeq (,$(BUILD_KERNEL))
BUILD_KERNEL=$(shell uname -r)
endif

###########################################################################
# Environment tests

KBASEDIR := /lib/modules/$(BUILD_KERNEL)
KDIR := $(KBASEDIR)/build

test_dir = $(shell [ -e $(dir)/include/linux ] && echo $(dir))
KDIR := $(foreach dir, $(KDIR), $(test_dir))

ifeq (,$(KDIR))
  KDIR := $(firstword $(KDIR))
endif

ifeq (,$(KDIR))
  $(warning *** Kernel header files not in any of the expected locations.)
  $(warning *** Install the appropriate kernel development package, e.g.)
  $(error kernel-devel, for building kernel modules and try again)
endif


ARCH := $(shell uname -m | sed 's/i.86/i386/')
ifneq ($(ARCH),x86_64)
  $(error This driver is only available for x86_64)
endif


# set the install path
INSTDIR ?= updates/drivers/input


###########################################################################
# Build rules

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install: default
	$(MAKE) INSTALL_MOD_DIR=$(INSTDIR) -C $(KDIR) M=$(PWD) modules_install

uninstall:
	if [ -e $(KBASEDIR)/$(INSTDIR)/$(DRIVER_NAME).ko ] ; then \
	    rm -f $(KBASEDIR)/$(DRIVER_NAME).ko ; \
	fi
	/sbin/depmod -a

.PHONY: clean install

clean:
	rm -rf $(DRIVER_NAME).ko $(DRIVER_NAME).o $(DRIVER_NAME).mod.c $(DRIVER_NAME).mod.o $(DRIVER_CFILES:.c=.o) \
	.*cmd .tmp_versions Module.markers Module.symvers modules.order

endif

