CURDIR = $(shell pwd)
LIBDIR = ./lib

LIBCSVDIR = libcsv-3.0.3
LIBCSV_NAME = libcsv.a

all: $(LIBDIR)/$(LIBCSV_NAME)


$(LIBCSVDIR)/Makefile:
		cd $(LIBCSVDIR); ./configure --prefix=$(CURDIR); cd ..

$(LIBCSVDIR)/libcsv.la: $(LIBCSVDIR)/Makefile
		cd $(LIBCSVDIR); make;	cd ..

$(LIBDIR)/$(LIBCSV_NAME): $(LIBCSVDIR)/libcsv.la
		cd $(LIBCSVDIR); make install;	cd ..

clean:
		rm -rf lib/*
		rm -rf include/csv.h
		cd $(LIBCSVDIR); make clean; cd ..
		rm -f $(LIBCSVDIR)/Makefile

