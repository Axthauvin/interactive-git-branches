CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
CXXFLAGS += -m64 -march=native
CXXFLAGS += -Iinclude

SRC = \
	main.c \
	src/git.c \
	src/interactive.c \
	src/popup.c \
	src/search.c

OBJ = $(SRC:.c=.o)

TARGET = gbs

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean