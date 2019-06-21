CPP=cpp
CXX=g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin


UNAME := $(shell uname)

DEBUG_MODE=TRUE#debug mode activated
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
TESTLDFLAGS     += -lgtest -lgtest_main  -lpthread#link to gteson csif command to run 
CPPFLAGS        += -std=c++11 #c++ version
PROJ_NAME       = proj3 #create a program that can run

TESTMANCALA_NAME    = testMancala #name it

MAIN_OBJ        = $(OBJ_DIR)/main.o #cpp file that need to complie

TESTMANCALA_OBJ     = $(OBJ_DIR)/testmancala.o #cpp file that need to complie

PROJ_OBJS       = $(OBJ_DIR)/a.o     #cpp file that need to complie
            
            
all: directories test $(BIN_DIR)/$(PROJ_NAME)
test: $(TESTBIN_DIR)/$(TESTMANCALA_NAME) #check the dependency and whether the file exits before compling
	$(TESTBIN_DIR)/$(TESTMANCALA_NAME) #

	#create the program to run in the bin fir
$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(PROJ_OBJS) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 
	#create the test result of testmancala in the test
$(TESTBIN_DIR)/$(TESTMANCALA_NAME): $(PROJ_OBJS) $(TESTMANCALA_OBJ)
	$(CXX) $(PROJ_OBJS) $(TESTMANCALA_OBJ) -o $(TESTBIN_DIR)/$(TESTMANCALA_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 
	#create .o file in the obj dir
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
.PHONY: directories
directories:# create program for users to run
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	
clean::
	-rm $(PROJ_OBJS) $(INC_DIR)/*.*~ $(SRC_DIR)/*.*~
	#remove
.PHONY: clean