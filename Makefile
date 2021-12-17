# Project Name
TARGET = DaisyDrone

# Sources
CPP_SOURCES = DaisyDrone.cpp

# Library Locations
LIBDAISY_DIR = ../../Documents/Daisy/DaisyExamples/libdaisy/
DAISYSP_DIR = ../../Documents/Daisy/DaisyExamples/daisysp/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
