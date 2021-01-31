include ../mk/Makefile.root.include
include ../mk/Makefile.libs.include
HIDE=

OS=$(shell uname -s)

C_OBJS += $(patsubst %.c,%.o, $(wildcard mock/*.c)) 
C_OBJS += $(patsubst %.c,%.o, $(wildcard mock/linux/*.c)) 



all:$(TARGET_BIN)



PROJ_CFLAGS	+= -D$(OS)
PROJ_CXXFLAGS += -std=c++17 -D$(OS)
PROJ_INC_PATHES +=	$(GTEST_INC_PATH) $(PROJ_ROOT_SRC_PATH)/src
PROJ_LIB_PATHES += 	$(GTEST_LIB_PATH)
PROJ_LIB_NAMES 	+=   gtest_main  pthread

$(TARGET_BIN):$(OBJS)
	$(DO_CPP_TARGET)

clean:
	$(DO_CLEAN)
