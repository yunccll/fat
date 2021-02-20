include ../mk/Makefile.root.include
include ../mk/Makefile.libs.include
HIDE=


PROJ_INC_PATHES +=	$(GTEST_INC_PATH) .
PROJ_LIB_PATHES += 	$(GTEST_LIB_PATH)
PROJ_LIB_NAMES 	+=   gtest_main  pthread
PROJ_CXXFLAGS += #-DUTEST

THIS_OBJS	:= $(patsubst %.cpp,%.o, $(wildcard ./helper/*.cpp))
THIS_OBJS	+= $(patsubst %.cpp,%.o, $(wildcard ./helper/unittest/*.cpp))
OBJS 	+= $(THIS_OBJS)


all:$(TARGET_BIN)

$(TARGET_BIN):$(OBJS)
	$(DO_CPP_TARGET)

clean:
	$(DO_CLEAN)
