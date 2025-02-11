CC = g++
CFLAGS = -lgmp -fopenmp
TARGET = test_main
SRC = ../src/add.cpp ../src/substract.cpp ../src/multiply.cpp ../src/utils.cpp ../src/BigInt.cpp generate_input.cpp test_main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)
clean:
	rm -f $(TARGET)

.PHONY: all clean