include make/all.mk
include make/flags.mk
include make/variables.mk
include make/functions.mk
include make/objects.mk
include make/special_rules.mk

# TODO: split up into per-directory Makefiles
SRC := $(shell find src/ -type f -name "*.cc" -not -name "*_test.cc")

$(eval $(call binary,gamebun,$(SRC)))
