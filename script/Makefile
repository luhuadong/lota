VERSION=1.0.0
SCRIPT=install.sh
PACKAGE=hello
RUN=helloinstall.run

all:
	tar jcvf $(PACKAGE).tar.bz2 $(PACKAGE)/*
	cat $(SCRIPT) $(PACKAGE).tar.bz2 > $(RUN)
	chmod +x $(RUN)

clean:
	rm $(PACKAGE).tar.bz2 
	rm $(RUN)

