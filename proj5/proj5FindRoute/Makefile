CPP=cpp
CXX=g++

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin

#Uncomment the next line to enter debug
#DEBUG_MODE=TRUE

EXPAT = expat

ifdef DEBUG_MODE
DEFINES  += -DDEBUG
CFLAGS   += -g -ggdb -D_GLIBCXX_DEBUG
endif

#idea from Professor Nitta and TA Nick

INCLUDE  += -I $(INC_DIR)
CFLAGS   +=  -Wall `pkg-config --cflags $(EXPAT)`
LDFLAGS  += `pkg-config --libs $(EXPAT)`
TESTLDFLAGS  +=  -lgtest -lgtest_main  `pkg-config --libs $(EXPAT)` -lpthread
CPPFLAGS += -std=c++14 

PROJ_NAME = findroute_extra

TEST_NAME = test

PROJ_OBJS = $(OBJ_DIR)/XMLReader.o      \
            $(OBJ_DIR)/MapRouter.o\
      		$(OBJ_DIR)/CSVReader.o\
		$(OBJ_DIR)/StringUtils.o\
		$(OBJ_DIR)/CSVWriter.o


				
MAIN_OBJS = $(OBJ_DIR)/findroute.o

TEST_OBJS = $(OBJ_DIR)/testrouter.o
            
all: directories test $(BIN_DIR)/$(PROJ_NAME)
test: $(TESTBIN_DIR)/$(TEST_NAME)
	$(TESTBIN_DIR)/$(TEST_NAME)

$(BIN_DIR)/$(PROJ_NAME): directories $(PROJ_OBJS) $(MAIN_OBJS)
	$(CXX) $(PROJ_OBJS) $(MAIN_OBJS) -lncurses lib/libcsv.a -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(TESTBIN_DIR)/$(TEST_NAME): directories $(PROJ_OBJS) $(TEST_OBJS)
	$(CXX) $(PROJ_OBJS) $(TEST_OBJS)  lib/libcsv.a -o $(TESTBIN_DIR)/$(TEST_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
.PHONY: directories
directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(TESTBIN_DIR)
	
.PHONY: cleanit
cleanit:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(TESTBIN_DIR)
	
.PHONY: clean
clean: cleanit
	


