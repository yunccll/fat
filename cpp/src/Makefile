include ../mk/Makefile.root.include
include ../mk/Makefile.libs.include


all:$(TARGET_BIN)

PROJ_INC_PATHES +=	$(GTEST_INC_PATH)
PROJ_LIB_PATHES += 	$(GTEST_LIB_PATH)
PROJ_LIB_NAMES 	+=   gtest_main  pthread


$(TARGET_BIN):$(OBJS)
	$(DO_CPP_TARGET)

clean:
	$(DO_CLEAN)
