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

file: NACA_ploter_eq.tex
	@pdflatex NACA_ploter_eq.tex > latex_temp.txt
	@makeindex NACA_ploter_eq.nlo -s nomencl.ist -o NACA_ploter_eq.nls > latex_temp.txt
	@pdflatex NACA_ploter_eq.tex > latex_temp.txt
	@pdflatex NACA_ploter_eq.tex > latex_temp.txt
	@cp -r *.log logs/.
	@rm *.aux *.ilg *.lof *.nlo *.nls *.out *.toc latex_temp.txt *.log *.fls *.fdb_latexmk *.lol *.bbl *.blg

clean_after_vscode:
	@test ! $(wildcard *.log) || cp -r *.log logs/.
	@rm *.fdb_latexmk *.fls *.synctex.gz *.aux *.bbl *.log *.blg *.toc *.out *.lof *.nlo *.lol