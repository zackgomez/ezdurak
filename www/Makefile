include ../Makefile.inc

all: site

site:
	mkdir -p files
	cp ../dist/ezdurak-linux.tar.gz files

deploy: site
	scp -r index.html screenshot.png files dkvirtual@software.dkgomez.com:~/software.dkgomez.com/ezdurak

clean:
	echo "clean up www"
	rm -rf files
