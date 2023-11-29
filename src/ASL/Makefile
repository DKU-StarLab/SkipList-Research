# Compiler and compiler flags
CXX = g++
CXXFLAGS = -Wall -g

# Define the target executable and object files
TARGET = sl_test
OBJS = skiplist_test.o zipf.o latest-generator.o

# Define how to compile the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Define how object files should be compiled from source files
skiplist_test.o: skiplist_test.cc skiplist.h zipf.h latest-generator.h
	$(CXX) $(CXXFLAGS) -c skiplist_test.cc

skiplist.o: skiplist.h

zipf.o: zipf.cc zipf.h
	$(CXX) $(CXXFLAGS) -c zipf.cc

latest-generator.o: latest-generator.cc latest-generator.h
	$(CXX) $(CXXFLAGS) -c latest-generator.cc

# Define the clean rule for cleaning up
clean:
	rm -f $(TARGET) $(OBJS)
