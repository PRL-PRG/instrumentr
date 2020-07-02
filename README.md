# instrumentr

<!-- badges: start -->
[![CRAN Status](https://www.r-pkg.org/badges/version/instrumentr)](https://cran.r-project.org/package=instrumentr)
[![Package Lifecycle](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
[![License: MIT](https://img.shields.io/github/license/PRL-PRG/instrumentr)](https://opensource.org/licenses/MIT)
[![Build Status](https://github.com/PRL-PRG/instrumentr/workflows/R-CMD-check/badge.svg)](https://github.com/PRL-PRG/instrumentr/actions)
[![lintr Status](https://github.com/PRL-PRG/instrumentr/workflows/lintr/badge.svg)](https://github.com/PRL-PRG/instrumentr/actions?query=workflow%3Alintr)
[![clang-tidy](https://github.com/PRL-PRG/instrumentr/workflows/clang-tidy/badge.svg)](https://github.com/PRL-PRG/instrumentr/actions?query=workflow%3Aclang-tidy)
[![cppcheck Status](https://github.com/PRL-PRG/instrumentr/workflows/cppcheck/badge.svg)](https://github.com/PRL-PRG/instrumentr/actions?query=workflow%3Acppcheck)
[![rchk](https://github.com/PRL-PRG/instrumentr/workflows/rchk/badge.svg)](https://github.com/PRL-PRG/instrumentr/actions?query=workflow%3Archk)
[![Code Coverage](https://codecov.io/gh/PRL-PRG/instrumentr/branch/master/graph/badge.svg)](https://codecov.io/gh/PRL-PRG/instrumentr)
<!-- badges: end -->


## Overview

[instrumentr](https://prl-prg.github.io/instrumentr/) is an instrumentation framework for R that helps you address common dynamic analysis problems. It exposes hooks for the following program execution events :

  - Application
    - Load
    - Unload
    - Attach
    - Detach

  - Package
    - Load
    - Unload
    - Attach
    - Detach

  - Function
    - Attach
    - Detach

  - Call
    - Entry
    - Exit

These hooks invoke user defined callbacks that receive the state of the running program in the form of model objects.
Model object properties can be queried using a comprehensive API at both R and C++ level.


## Setup

To install the latest version, run the following R command:

```r
devtools::install_github("PRL-PRG/instrumentr")
```

To uninstall the existing version, run the following R command:

```r
remove.packages("instrumentr")
```


## Getting help

Please report bugs with a minimal reproducible example at [GitHub](https://github.com/PRL-PRG/instrumentr/issues).

Request new features with use cases at [GitHub](https://github.com/PRL-PRG/instrumentr/issues).

For questions please contact [Aviral Goel](http://aviral.io).

