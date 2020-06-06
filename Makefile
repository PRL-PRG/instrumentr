.PHONY: all build check document test

all: document build check

build: document
	R CMD build .

check: build
	R CMD check lightr*tar.gz

clean:
	-rm -f lightr*tar.gz
	-rm -fr lightr.Rcheck
	-rm -rf src/*.o src/*.so

document:
	Rscript -e 'devtools::document()'

test:
	Rscript -e 'devtools::test()'

lintr:
	R --slave -e "lintr::lint_package()"

clang-analyze:
	scan-build make build
