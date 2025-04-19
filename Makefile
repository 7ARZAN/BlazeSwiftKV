NAME        = hotrace
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -O3 -Ofast -march=native -funroll-loops
SRCS        = hash_methods.c hotrace.c hr_tools.c main.c init.c hr_utils.c
OBJS        = build/hash_methods.o build/hotrace.o build/hr_tools.o build/main.o build/init.o build/hr_utils.o
HEADER      = hotrace.h
RM          = rm -rf
PYTHON      = python3
GENERATOR   = input_creator.py 
TEXT_FILE   = hotrace.txt

C_GREEN     = \033[1;32m
C_CYAN      = \033[1;36m
C_RED       = \033[1;31m
C_YELLOW    = \033[1;33m
C_RESET     = \033[0m

SHELL       = /bin/zsh

all: build $(NAME) input
	@echo -e "$(C_GREEN)[+] Done: hotrace, hotrace.txt created$(C_RESET)"

$(NAME): $(OBJS) $(HEADER)
	@echo -e "\r$(C_GREEN)[+] Done: hotrace executable created$(C_RESET)"
	@$(CC) $(OBJS) -o $(NAME)

build/%.o: %.c $(HEADER)
	@echo -e "\r$(C_GREEN)[+] Done: $@ created$(C_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

build:
	@echo -e "\r$(C_CYAN)[+] Done: build/ directory created$(C_RESET)"
	@if [ ! -d "build/" ]; then mkdir build; fi

input: $(TEXT_FILE)
	@echo -e "$(C_GREEN)[+] Done: Input generation complete$(C_RESET)"

.INTERMEDIATE: $(GENERATOR)
	@if [ ! -f "$@" ]; then @$(RM) input_creator.py 2>/dev/null; fi

$(TEXT_FILE): $(GENERATOR)
	@echo -e "\r$(C_YELLOW)[+] Done: hotrace.txt created$(C_RESET)"
	@$(PYTHON) $(GENERATOR)

$(GENERATOR):
	@echo -e "\r$(C_RED)[+] Done: input_creator.py created$(C_RESET)"
	@printf '%s\n' \
		'import random' \
		'' \
		'RANDOM_NUM = 1000000' \
		'animals = [f"animal{i}" for i in range(RANDOM_NUM)]' \
		'sounds = [f"sound{i}" for i in range(RANDOM_NUM)]' \
		'' \
		'dictionary_section = "\n".join(f"{animal}\n{sound}" for animal, sound in zip(animals, sounds))' \
		'' \
		'extra_animals = animals + [f"unknown{i}" for i in range(RANDOM_NUM)]' \
		'search_animals = [random.choice(extra_animals) for _ in range(RANDOM_NUM)]' \
		'search_section = "\n".join(search_animals)' \
		'' \
		'hotrace_file = f"{dictionary_section}\n\n{search_section}\n"' \
		'' \
		'with open("hotrace.txt", "w") as f:' \
		'    f.write(hotrace_file)' \
		'' \
		'print("hotrace.txt created !! exec : ./hotrace < hotrace.txt")' \
		> $(GENERATOR)

clean:
	@echo -e "\r$(C_YELLOW)[+] Done: build/ directory deleted$(C_RESET)"
	@$(RM) build

fclean: clean
	@echo -e "\r$(C_RED)[+] Done: hotrace, hotrace.txt deleted$(C_RESET)"
	@$(RM) $(NAME) $(TEXT_FILE) 

re: fclean all

.PHONY: all clean fclean re input
