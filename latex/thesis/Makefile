NOTE  = !! change the next line to fit your filename; no spaces at file name end !!
FILE  = Thesis

all:
	latex $(FILE)
	latex $(FILE)
	latex $(FILE)
	dvips -Pcmz $(FILE).dvi -o $(FILE).ps
	ps2pdf $(FILE).ps $(FILE).pdf
	
clean:
	rm -f *.dvi *.log *.aux *.bbl *.blg *.toc *.lof *.lot *.cb *.~



