################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include bitcoin_bignum/subdir.mk
-include subdir.mk
-include objects.mk

DEBUG_FLAGS=	-g
CXXFLAGS=	-fopenmp -I/opt/local/include -I/usr/local/include -Wall -fpic
LDFLAGS=	-L /opt/local/lib -L /usr/local/lib


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

 -include ../makefile.defs

.DEFAULT_GOAL := libzerocoin.so

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libzerocoin.so test tutorial

# Tool invocations
libzerocoin.so: $(OBJS) $(USER_OBJS)
	${CXX} ${LDFLAGS} -shared -o "libzerocoin.so" $(OBJS) $(USER_OBJS) $(LIBS)

test: $(OBJS) $(USER_OBJS) 
	${CXX} ${LDFLAGS} -o "tests" ../Tests.cpp $(OBJS) $(USER_OBJS) $(LIBS)

paramgen: $(OBJS) $(USER_OBJS)
	${CXX} ${LDFLAGS} -o "paramgen" ../paramgen.cpp $(OBJS) $(USER_OBJS) $(LIBS)

benchmark: $(OBJS) $(USER_OBJS) 
	${CXX} ${LDFLAGS} -o "benchmark" ../Benchmark.cpp $(OBJS) $(USER_OBJS) $(LIBS)

tutorial: $(OBJS) $(USER_OBJS) 
	${CXX} ${LDFLAGS} -o "tutorial" ../Tutorial.cpp $(OBJS) $(USER_OBJS) $(LIBS)

# Other Targets
clean:
	-$(RM) -f $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libzerocoin.so tests benchmark paramgen

.PHONY: all clean dependents
.SECONDARY:

 -include ../makefile.targets
