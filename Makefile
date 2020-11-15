CPPFLAGS := -O3 -Wall -Wextra -Werror
LDFLAGS := -lcurl
# add -static for statically linked?

SRC := src
OUT := build

OBJS := $(patsubst $(SRC)/%.cpp, $(OUT)/%.o,\
    $(shell find $(SRC) -name "*.cpp"))

default: ddnsd++

$(OUT)/%.o: $(SRC)/%.cpp
	@echo [CXX] $<
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

ddnsd++: $(OBJS)
	@echo [LNK] $@
	@$(CXX) $(CPPFLAGS) -o ddnsd++ $(OBJS) $(LDFLAGS)

clean:
	rm -rf build ddnsd++
