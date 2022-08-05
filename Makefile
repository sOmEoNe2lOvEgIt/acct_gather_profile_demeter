##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

SRC =	src/demeter.c\

CFLAGS = -DHAVE_CONFIG_H -Wall -Wextra -Wpedantic -g -O2 -pthread -fno-gcse -g -O0 -fno-strict-aliasing -MT prep_demeter.lo -MD -MP -MF .deps/prep_demeter.Tpo -fPIC -DPIC

LDFLAGS = -lslurm

OBJ =	$(SRC:.c=.o)

NAME =	prep_demeter

$(NAME):
		gcc -I ./include -I ~/SLURM/slurm.build -o $(OBJ) -c $(SRC) $(CFLAGS)
		mv src/*.o ./.libs/
		mv -f .deps/prep_demeter.Tpo .deps/prep_demeter.Plo
		rm -fr .libs/prep_demeter.a .libs/prep_demeter.la .libs/prep_demeter.lai prep_demeter.so
		gcc -shared .libs/prep_demeter.o -O2 -pthread -O0 -pthread -Wl,-soname -Wl,prep_demeter.so -o prep_demeter.so

all:	$(NAME)

clean:
		@rm -f $(OBJ)
		@rm -f prep_demeter.so

fclean:	clean
		@rm -f $(NAME)

re:		clean   all