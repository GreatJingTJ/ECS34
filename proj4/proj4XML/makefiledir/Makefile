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
PROJ_NAME       = proj4 #create a program that can run


testCSV_NAME    = testCSV #name it

testXML_NAME    =  testXML

MAIN_OBJ        = $(OBJ_DIR)/main.o #cpp file that need to complie

testCSV_OBJ     = $(OBJ_DIR)/testXML.o #cpp file that need to complie

testXML_OBJ     =  $(OBJ_DIR)/testXML.o

CSV_READER_OBJ  = $(OBJ_DIR)/XMLReader.o

CSV_WRITER_OBJ  = $(OBJ_DIR)/XMLWriter.o

XML_Reader      = $(OBJ_DIR)/XMLReader.o

XML_Writer      = $(OBJ_DIR)/XMLWriter.o

PROJ_OBJS       = $(OBJ_DIR)/XMLReader.o\
		  $(OBJ_DIR)/XMLWriter.o\
		  $(OBJ_DIR)/CSVReader.o\
		  $(OBJ_DIR)/CSVWriter.o
	
all: directories test $(BIN_DIR)/$(PROJ_NAME)
test: $(TESTBIN_DIR)/$(testCSV_NAME) 
	$(TESTBIN_DIR)/$(testCSV_NAME)

	#create the program to run in the bin fir
$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(PROJ_OBJS) lib/libcsv.a -lexpat  -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 
	#create the test result of testCSV in the test
$(TESTBIN_DIR)/$(testCSV_NAME): $(PROJ_OBJS) $(testCSV_OBJ)
	$(CXX) $(PROJ_OBJS) $(testCSV_OBJ) lib/libcsv.a -o $(TESTBIN_DIR)/$(testCSV_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 
	#create .o file in the obj dir

$(TESTBIN_DIR)/$(testCSV_NAME): $(PROJ_OBJS) $(testXML_OBJ)
        $(CXX) $(PROJ_OBJS) $(testXML_OBJ) -lexpat -o $(TESTBIN_DIR)/$(testXML_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS)

$(CSV_READER_OBJ): $(SRC_DIR)/CSVReader.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/CSVReader.cpp -o $(CSV_READER_OBJ)

$(CSV_WRITER_OBJ): $(SRC_DIR)/CSVWriter.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/CSVWriter.cpp -o $(CSV_WRITER_OBJ)

$(XML_Reader): $(SRC_DIR)/XMLReader.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/XMLReader.cpp -o $(XML_Reader)

$(XML_Reader): $(SRC_DIR)/XMLReader.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c src/XMLReader.cpp -o $(XML_Reader)
	

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
