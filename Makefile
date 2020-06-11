.PHONY: all build check document test

all: document build check

build: document
	R CMD build .

check: build
	R CMD check instrumentr*tar.gz

clean:
	-rm -f instrumentr*tar.gz
	-rm -fr instrumentr.Rcheck
	-rm -rf src/*.o src/*.so

document:
	Rscript -e 'devtools::document()'

test:
	Rscript -e 'devtools::test()'

lintr:
	R --slave -e "quit(status = length(print(lintr::lint_package())) != 0)"

clang-analyze:
	scan-build make build
