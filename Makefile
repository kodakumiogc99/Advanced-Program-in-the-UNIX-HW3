FINAL=libmini
SRC_FILE=$(basename $(notdir $(filter-out libmini.c, $(wildcard *.c))))
TEST_OBJ=$(patsubst %.c, %.o, $(wildcard *.c))
OBJ_FILE=$(patsubst %.asm, %.o, $(wildcard *.asm)) libmini.o
ARCHIVE=$(FINAL).a
SO_FILE=$(FINAL).so

CFLAG=-c -g -Wall -fno-stack-protector -nostdlib
CFLAG1=-fPIC
CFLAG2=-I. -I.. -DUSEMINI
CC=gcc
AS=yasm
AFLAG=-f elf64 -DYASM -D__x86_64__ -DPIC

LD_SRC=libmini.a start.o -L. -L..
LFLAG=-m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2


all: $(ARCHIVE) $(SO_FILE) $(OBJ_FILE) $(SRC_FILE)
	@echo $(.DEFAULT_GOAL)
	@echo 'SUCCESFULL'

$(ARCHIVE): libmini64.o libmini.o
	@echo ARCHIVE $@ from $^
	ar rc $@ $^
	@echo OK..

$(SO_FILE): libmini64.o libmini.o
	@echo LOAD
	ld -shared $^ -o $@
	@echo OK..


%.o: %.asm
	@echo Compile $< to $@. '(%.asm > %.o)'
	$(AS) $(AFLAG) $< -o $@
	@echo OK..

%.o: %.c
	@echo Compile $< to $@. '(%.c > %.o)'
	$(CC) $(CFLAG) $(CFLAG1) $< -o $@
	@echo OK..

%: %.c
	@echo Compile $< to $@. '(%.c > %)'
	$(CC) $(CFLAG) $(CFLAG2) $< -o $@.o
	ld $(LFLAG) -o $@ $@.o $(LD_SRC)
	@echo OK..

clean:
	rm -rf $(OBJ_FILE) $(ARCHIVE) $(SO_FILE) $(SRC_FILE) $(TEST_OBJ)
