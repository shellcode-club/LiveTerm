##
## SHELLCODE
## LiveTerm, 2021
##

SRC         =   $(wildcard src/**.c)
OBJ         =   $(SRC:.c=.o)
PREFIX      =   /usr/bin
NAME        =   liveterm

CFLAGS      =   -Wall                   \
                -W                      \
                -Wno-unused-parameter   \
                -I .

all:    $(NAME)

$(NAME):$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS)

install:
	cp $(NAME) $(PREFIX)/$(NAME)

uninstall:
	rm -f $(PREFIX)/$(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:		fclean all
