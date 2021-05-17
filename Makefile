R := R
SOURCEDIR := src
INCLUDEDIR := inst/include
DOCKER := docker

SOURCES := $(shell find $(SOURCEDIR) -name '*.cpp')
INCLUDES := $(shell find $(INCLUDEDIR) -name '*.hpp') $(shell find $(SOURCEDIR) -name '*.h')

CPPCHECK := cppcheck
SCAN_BUILD := scan-build

.PHONY: all build check document test

all: clean document build check

build: install-dplyr document
	$(R) CMD build .

check: build
	$(R) CMD check instrumentr*tar.gz

clean:
	-rm -f instrumentr*tar.gz
	-rm -fr instrumentr.Rcheck
	-rm -rf src/*.o src/*.so

install: clean
	$(R) CMD INSTALL .

uninstall:
	$(R) -s -e "remove.packages('instrumentr')"

generate-callback-api: install-stringr
	$(R) -s --file=inst/meta/generate-callback-api.R --args R/callbacks.R

document: generate-callback-api install-devtools
	$(R) -s -e "devtools::document()"

test: install-devtools
	$(R) -s -e "devtools::test()"

lintr: install-lintr
	$(R) -s -e "quit(status = length(print(lintr::lint_package())) != 0)"

install-devtools:
	$(R) -s -e "if (!require('devtools')) install.packages('devtools', repos = 'http://cran.rstudio.com/')"

install-stringr:
	$(R) -s -e "if (!require('string')) install.packages('stringr', repos = 'http://cran.rstudio.com/')";

install-lintr:
	$(R) -s -e "if (!require('lintr')) install.packages('lintr', repos = 'http://cran.rstudio.com/')"

install-dplyr:
	$(R) -s -e "if (!require('dplyr')) install.packages('dplyr', repos = 'http://cran.rstudio.com/')"

clang-tidy: clean
	$(SCAN_BUILD) make build

cppcheck:
	$(CPPCHECK) --version
	@$(CPPCHECK) --force                                                  \
	             --enable=all                                             \
	             --language=c++                                           \
	             --inconclusive                                           \
	             --std=c++11                                              \
	             --std=c++14                                              \
	             --inline-suppr                                           \
	             --error-exitcode=1                                       \
	             --quiet                                                  \
	             --suppress=missingIncludeSystem                          \
	             --suppress=unusedFunction                                \
	             --suppress=useStlAlgorithm                               \
	              -I inst/include/                                        \
	             $(SOURCES)                                               \
	             $(INCLUDES)

build-docker:
	$(DOCKER) build -t instrumentr  .

run-docker:
	$(DOCKER) run -it instrumentr
