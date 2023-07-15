# Detect Kernel Name
KERNEL := $(shell uname -s)

# Detect Architecture Name
ARCHITECTURE := $(shell uname -m)

# Set variables to depend on OS and architecture
ifeq ($(KERNEL), Linux)
	ifeq ($(ARCHITECTURE), x86_64)
		SYSTEM_DIR := x86-64_linux
	endif

	CPLEX_ROOT_DIR := /opt/ibm/ILOG/CPLEX_Studio2211
	CCC := g++
	CCOPT := -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG
	CCLNFLAGS := -lconcert -lilocplex -lcplex -lm -lpthread -ldl

else ifeq ($(KERNEL), Darwin)
	ifeq ($(ARCHITECTURE), arm64)
		SYSTEM_DIR := arm64_osx
	else ifeq ($(ARCHITECTURE), x86_64)
		SYSTEM_DIR := x86-64_osx
	endif

	CPLEX_ROOT_DIR := /Applications/CPLEX_Studio2211
	CCC := clang++
	CCPOT := -m64 -O -fPIC -fexceptions -DNDEBUG -stdlib=libc++
	CCLNFLAGS := -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libc++

else
	$(error ERROR: OS or CPU architecture does not supported.)
endif

# Please remove comment out and set the variable according to your environment like examples below.
# CPLEX_ROOT_DIR := /opt/ibm/ILOG/CPLEX_Studio****
# CPLEX_ROOT_DIR := /Applications/CPLEX_Studio****

# Set Directories
LIBFORMAT := static_pic

CPLEXDIR		:= $(CPLEX_ROOT_DIR)/cplex
CONCERTDIR	:= $(CPLEX_ROOT_DIR)/concert

CPLEXLIBDIR		:= $(CPLEXDIR)/lib/$(SYSTEM_DIR)/$(LIBFORMAT)
CONCERTLIBDIR	:= $(CONCERTDIR)/lib/$(SYSTEM_DIR)/$(LIBFORMAT)

CPLEXINCDIR		:= $(CPLEXDIR)/include
CONCERTINCDIR	:= $(CONCERTDIR)/include

# Set Flags
CCLNDIRS := -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)

MYFLAGS := -std=c++20 -Wall -Wextra -Wshadow -ftrapv
CCFLAGS := $(MYFLAGS) $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)

# Set Files
BINDIR := bin
TARGET := $(BINDIR)/main.out

SRCDIR := src
SRCS := $(wildcard $(SRCDIR)/*.cpp)

OBJDIR := obj
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

OUTDIR := out

MAKE_DIRS := $(BINDIR) $(OBJDIR) $(OUTDIR)

# Set Commands
all: $(MAKE_DIRS) $(TARGET)

rebuid: clean all

execute: all run

reexecute: clean execute

run:
	./$(TARGET)

clean:
	rm -rf $(MAKE_DIRS)

$(TARGET): $(OBJS)
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o $@ $^ $(CCLNFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CCC) -c $(CCFLAGS) $^ -o $@

$(MAKE_DIRS):
	$(shell mkdir $@)
