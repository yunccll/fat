APP_PATH= .
INC_PATH= 	$(APP_PATH) \
			#$(APP_PATH)/include

LIB_PATH= 	$(APP_PATH) \
			#$(APP_PATH)/lib

#DYNAMIC_LIBS= rt dl
STATIC_LIBS= 

SRC_NAME = $(patsubst %.cpp, %, $(wildcard *.cpp))   $(patsubst %.c, %, $(wildcard *.c)) 

TARGET= main

CC=gcc

DEBUG_FLAGS= -g -Werror #-std=gnu99#-Wwrite-strings -Werror

CFLAGS= $(addprefix -I, $(INC_PATH)) $(DEBUG_FLAGS)
LFLAGS= $(DEBUG_FLAGS) $(addprefix -L, $(LIB_PATH))  $(addprefix -l, $(STATIC_LIBS))
#LFLAGS= $(DEBUG_FLAGS)
OBJS=$(addsuffix .o, $(SRC_NAME))

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS) 

%.o:%.c
	$(CC) $(CFLAGS) -c $<

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	@rm -f $(TARGET) $(OBJS) *.o 


