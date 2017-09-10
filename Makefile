APP_PATH= .
INC_PATH= 	$(APP_PATH) \
			$(APP_PATH)/test

LIB_PATH= 	$(APP_PATH)

#DYNAMIC_LIBS= rt dl
STATIC_LIBS= 

SRC_NAME = 	$(patsubst %.cpp, %, $(wildcard *.cpp))   \
			$(patsubst %.c, %, $(wildcard *.c)) 	\
			$(patsubst %.c, %, $(wildcard test/*.c))

TARGET= bin/main

CC=gcc

DEBUG_FLAGS= -g -Werror

CFLAGS= $(addprefix -I, $(INC_PATH)) $(DEBUG_FLAGS)
LFLAGS= $(DEBUG_FLAGS) $(addprefix -L, $(LIB_PATH))  $(addprefix -l, $(STATIC_LIBS))
OBJS=$(addsuffix .o, $(SRC_NAME))

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS) 

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(TARGET) $(OBJS) *.o 


