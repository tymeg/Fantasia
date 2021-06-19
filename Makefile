# Makefile dla gry Fantasia by Tymeg

CC=g++
CFLAGS = -std=c++14 -Wall -Wextra -Werror
# ogolne flagi kompilacji dla modulow

LFLAGS = -lm
# ogolne flagi konsolidacji programu

NAME = FANTASIA   # nazwa programu wynikowego
OBJS = game.o main.o   # nazwy wszystkich modułów (binarnych)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS)  

game.o: game.cpp game.h system.h
	$(CC) $(CFLAGS) -c game.cpp -o game.o	

main.o: main.cpp game.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o

clean:
	rm -f $(OBJS) $(NAME)
