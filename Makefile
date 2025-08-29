CC = x86_64-w64-mingw32-gcc
CFLAGS = -Iinclude
TARGET = writing_win_code_pls_dont_bop_me_soc
# TARGET = b
SRC = src
BUILD = build

# Goal is to do each first 

all: $(TARGET)
# all: $(BUILD)/part-1.o

# $(TARGET): $(BUILD)/part-1.o $(BUILD)/printer.o
# 	$(CC) -o $(TARGET) $(BUILD)/part-1.o $(BUILD)/printer.o $(CFLAGS)
#

# TODO: Make this less redundant because I will have to keep on adding .o files which is bad

$(TARGET): $(BUILD)/main.o $(BUILD)/printer.o $(BUILD)/part-1.o $(BUILD)/part-2.o $(BUILD)/part-3.o
	$(CC) -o $(TARGET) $(BUILD)/main.o $(BUILD)/printer.o $(BUILD)/part-1.o $(BUILD)/part-2.o $(BUILD)/part-3.o $(CFLAGS)

$(BUILD)/main.o: $(SRC)/main.c $(BUILD)/printer.o $(BUILD)/part-1.o $(BUILD)/part-2.o $(BUILD)/part-3.o
	$(CC) -o $@ -c $(SRC)/main.c $(CFLAGS)

$(BUILD)/part-3.o: $(SRC)/part-3.c $(BUILD)/printer.o
	$(CC) -o $@ -c $(SRC)/part-3.c $(CFLAGS)

$(BUILD)/part-2.o: $(SRC)/part-2.c $(BUILD)/printer.o
	$(CC) -o $@ -c $(SRC)/part-2.c $(CFLAGS)

$(BUILD)/part-1.o: $(SRC)/part-1.c $(BUILD)/printer.o
	$(CC) -o $@ -c $(SRC)/part-1.c $(CFLAGS)

# $(TARGET): $(SRC)/printer.c
$(BUILD)/printer.o: $(SRC)/printer.c
	$(CC) -c $(SRC)/printer.c -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(TARGET).exe
	rm -f $(BUILD)/*


# what is a better way to do this?
