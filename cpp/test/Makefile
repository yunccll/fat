include ../mk/Makefile.root.include
include ../mk/Makefile.libs.include
HIDE=


PROJ_INC_PATHES +=	$(GTEST_INC_PATH) .
PROJ_LIB_PATHES += 	$(GTEST_LIB_PATH)
PROJ_LIB_NAMES 	+=   gtest_main  pthread
PROJ_CXXFLAGS += -std=c++14 #-DUTEST


PROJ_CXX_SRC += $(wildcard ./helper/*.cpp)
PROJ_CXX_SRC += $(wildcard ./helper/unittest/*.cpp)
PROJ_CXX_SRC += $(wildcard ./helper/object/*.cpp)



all:$(TARGET_BIN)

$(TARGET_BIN):$(OBJS)
	$(DO_CPP_TARGET)

clean:
	$(DO_CLEAN)

utest: nothing
	make -C ./utest

utest.clean: nothing
	make -C ./utest clean

nothing:

