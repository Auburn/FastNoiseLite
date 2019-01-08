CXX ?= g++
LINK ?= ar
CXXFLAGS ?= -std=c++11 \
            -fPIC -msse \
			-O3 -c
LINKFLAGS = rs

LIB_DIR = build

all: $(LIB_DIR)/libFastNoise.a

# make object files from source
.INTERMEDIATE: $(LIB_DIR)/FastNoise.o
$(LIB_DIR)/FastNoise.o: FastNoise.cpp
	$(CXX) $(CXXFLAGS) -o $(@) $<

# link object files into library
$(LIB_DIR)/libFastNoise.a: $(LIB_DIR)/FastNoise.o
	$(LINK) $(LINKFLAGS) $(@) $<

clean:
	rm -rf $(LIB_DIR)/*
