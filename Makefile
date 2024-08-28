FLAGS = -Ofast -flto -std=c++20 -mavx2 -g -march=native
SRC = main.cc
TARGET = a.out

all: $(TARGET)

run: $(TARGET)
	./$^

clean:
	rm -f $(TARGET)

$(TARGET): $(SRC)
	g++ $^ $(FLAGS) -o $@

thingy.S: $(SRC)
	g++ $^ $(filter-out -flto,$(FLAGS)) -S -o $@

.PHONY: all run clean