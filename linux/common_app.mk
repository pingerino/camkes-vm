#
# Copyright 2016, Data 61
#
# This software may be distributed and modified according to the terms of
# the GNU General Public License version 2. Note that NO WARRANTY is provided.
# See "LICENSE_GPLv2.txt" for details.
#
# @TAG(D61_GPL)
#

CURRENT_DIR := $(dir $(abspath $(lastword ${MAKEFILE_LIST})))
LDFLAGS += -L$(CURRENT_DIR)out/static_lib