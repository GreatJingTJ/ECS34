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
CPPFLAGS        += -std=c++14 #c++ version

testCSV_NAME    = testXML #name it


testCSV_OBJ     = $(OBJ_DIR)/testXML.o #cpp file that need to complie

XML_READER_OBJ  = $(OBJ_DIR)XMLReader.o

XML_WRITER_OBJ  = $(OBJ_DIR)XMLWriter.o

PROJ_OBJS       =   $(OBJ_DIR)XMLWriter.o\
					$(OBJ_DIR)XMLReader.o
					          			 
            
all: directories test
test: $(TESTBIN_DIR)/$(testCSV_NAME) #check the dependency and whether the file exits before compling
	$(TESTBIN_DIR)/$(testCSV_NAME) #

$(TESTBIN_DIR)/$(testCSV_NAME): $(PROJ_OBJS) $(testCSV_OBJ)
	$(CXX) $(PROJ_OBJS) $(testCSV_OBJ) -lexpat -o $(TESTBIN_DIR)/$(testCSV_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 

$(XML_READER_OBJ): $(SRC_DIR)/XMLReader.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/XMLReader.cpp -o $(XML_READER_OBJ)

$(XML_WRITER_OBJ): $(SRC_DIR)/XMLWriter.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/XMLWriter.cpp -o $(XML_WRITER_OBJ)


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

