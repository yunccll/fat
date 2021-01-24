include ../mk/Makefile.root.include
include ../mk/Makefile.libs.include
HIDE=

OS=$(shell uname -s)


all:$(TARGET_BIN)


PROJ_CFLAGS	+= -D$(OS)
PROJ_CXXFLAGS += -std=c++17 -D$(OS)
PROJ_INC_PATHES +=	$(GTEST_INC_PATH) 
PROJ_LIB_PATHES += 	$(GTEST_LIB_PATH)
PROJ_LIB_NAMES 	+=   gtest_main  pthread

$(TARGET_BIN):$(OBJS)
	$(DO_CPP_TARGET)

clean:
	$(DO_CLEAN)
