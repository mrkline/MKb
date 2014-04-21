# Taken largely from http://scottmcpeak.com/autodepend/autodepend.html
# I mean to mess with another build systems (maybe scons) at some point,
# but will do just fine until then

CXXFLAGS := -std=c++11 -Wall -Wextra -Weffc++ -pedantic
LIBFLAGS :=

OBJS := $(filter-out src/main.o, $(patsubst %.cpp,%.o, $(wildcard src/*.cpp)))
TESTOBJS := $(patsubst %.cpp,%.o, $(wildcard tests/*.cpp))

unit_tests: CXXFLAGS += -I. -Isrc -Itests -g
unit_tests: $(OBJS) $(TESTOBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(TESTOBJS) $(LIBFLAGS) -o unit_tests

# pull in dependency info for *existing* .o files
-include $(OBJS:.o=.d)
-include $(TESTOBJS:.o=.d)

# For if we used precomipled headers later
# precomp.hpp.gch: precomp.hpp
# 	$(CXX) $(CXXFLAGS) precomp.hpp

# compile and generate dependency info;
# more complicated dependency computation, so all prereqs listed
# will also become command-less, prereq-less targets
#   sed:    strip the target (everything before colon)
#   sed:    remove any continuation backslashes
#   fmt -1: list words one per line
#   sed:    strip leading spaces
#   sed:    add trailing colons
# %.o: precomp.hpp.gch %.cpp
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

# remove compilation products
clean:
	rm -f tests/*.o common/*.o *.o *.gch *.d unit_tests*

.PHONY: clean
