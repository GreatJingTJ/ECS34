AR=ar
AS=as
CC=gcc
CPP=cpp
CXX=g++
LD=ld
OBJCOPY = objcopy
OBJDUMP = objdump
STRIP = strip

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin


UNAME := $(shell uname)

PKGS = 

DEBUG_MODE=TRUE
ifdef DEBUG_MODE
DEFINES  += -DDEBUG
CFLAGS   += -g -ggdb -D_GLIBCXX_DEBUG
else 
CFLAGS   += -O3
endif

ifeq ($(UNAME), Darwin)
# add paths if developing on mac with macports
INCLUDE     += -I '/opt/local/include'
TESTLDFLAGS += -L '/opt/local/lib'
endif

INCLUDE         += -I $(INC_DIR)
CFLAGS          +=  -Wall
LDFLAGS         += 
TESTLDFLAGS     += -lgtest -lgtest_main  -lpthread
CPPFLAGS        += -std=c++14 
PROJ_NAME       = proj1
TESTSTR_NAME    = teststr
TESTPATH_NAME   = testpath

MAIN_OBJ        = $(OBJ_DIR)/main.o

TESTSTR_OBJ     = $(OBJ_DIR)/teststrutils.o
TESTPATH_OBJ    = $(OBJ_DIR)/testpath.o

PROJ_OBJS       = $(OBJ_DIR)/StringUtils.o        \
                  $(OBJ_DIR)/Path.o
      
            
            
all: directories test $(BIN_DIR)/$(PROJ_NAME)
test: $(TESTBIN_DIR)/$(TESTSTR_NAME) $(TESTBIN_DIR)/$(TESTPATH_NAME)
	$(TESTBIN_DIR)/$(TESTSTR_NAME)
	$(TESTBIN_DIR)/$(TESTPATH_NAME)
	
$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(PROJ_OBJS) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 
	
$(TESTBIN_DIR)/$(TESTSTR_NAME): $(PROJ_OBJS) $(TESTSTR_OBJ)
	$(CXX) $(PROJ_OBJS) $(TESTSTR_OBJ) -o $(TESTBIN_DIR)/$(TESTSTR_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 
	
$(TESTBIN_DIR)/$(TESTPATH_NAME): $(PROJ_OBJS) $(TESTPATH_OBJ)
	$(CXX) $(PROJ_OBJS) $(TESTPATH_OBJ) -o $(TESTBIN_DIR)/$(TESTPATH_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	
clean::
	-rm $(PROJ_OBJS) $(MAIN_OBJ) $(TESTSTR_OBJ) $(TESTPATH_OBJ) $(INC_DIR)/*.*~ $(SRC_DIR)/*.*~
	
.PHONY: clean