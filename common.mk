# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=

# Next lines are for C++ projects only

EXTRA_SUFFIXES+=cxx cpp

LDFLAGS+=-lang-c++

VFLAG_g=-gstabs+

EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/tests $(PROJECT_ROOT)/src/hal  \
	$(PROJECT_ROOT)/src/milestone1  \
	$(PROJECT_ROOT)/src/milestone2  \
	$(PROJECT_ROOT)/src/examples $(PROJECT_ROOT)/src/timer

LIBS+=ioaccess

EXTRA_INCVPATH+=$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/tests $(PROJECT_ROOT)/src/hal  \
	$(PROJECT_ROOT)/src/milestone1

CCFLAGS+=-Wall -DSIMULATION

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

