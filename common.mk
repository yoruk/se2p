# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=

# Next lines are for C++ projects only

EXTRA_SUFFIXES+=cxx cpp

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-lang-c++

VFLAG_g=-gstabs+

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/tests $(PROJECT_ROOT)/src/hal  \
	$(PROJECT_ROOT)/src/milestone1  \
	$(PROJECT_ROOT)/src/milestone2  \
	$(PROJECT_ROOT)/src/examples $(PROJECT_ROOT)/src/timer

#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=ioaccess

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/tests $(PROJECT_ROOT)/src/hal  \
	$(PROJECT_ROOT)/src/milestone1

#===== CCFLAGS - add the flags to the C compiler command line. 
CCFLAGS+=-Wall

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

