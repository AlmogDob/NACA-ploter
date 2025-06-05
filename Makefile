
CFLAGS = -Wall -Wextra -lm -lSDL2 -lSDL2_ttf
CCHECKS = -fsanitize=address

temp: build_temp run_temp clean_temp  
	@echo [INFO] done

build_temp: temp.c
	@echo [INFO] building
	@gcc ./temp.c $(CFLAGS) -o temp

run_temp:
	@echo
	./temp

clean_temp:
	@echo
	rm temp

debug_build_temp: temp.c
	@gcc ./temp.c $(CFLAGS) -g -o temp
# valgrind -s --leak-check=full ./temp
# cloc --exclude-lang=JSON,make .
