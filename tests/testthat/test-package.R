
test_that("get_name returns correct package name", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            data$name <- get_name(package)
        }
    )

    trace_code(tracer, {
        library(MASS)
        unloadNamespace("MASS")
    })

    data <- get_data(tracer)

    expect_identical(data$name, "MASS")
})


test_that("get_directory returns correct package directory", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            data$directory <- get_directory(package)
        }
    )

    trace_code(tracer, {
        library(MASS)
        unloadNamespace("MASS")
    })

    data <- get_data(tracer)

    expect_identical(data$directory, installed.packages()["MASS", "LibPath"])
})

test_that("get_namespace returns correct package namespace", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            namespace <- get_namespace(package)
            data$received_namespace <- get_object_details(namespace)$address
            data$actual_namespace <- get_object_details(getNamespace("MASS"))$address
        }
    )

    trace_code(tracer, {
        library(MASS)
        unloadNamespace("MASS")
    })

    data <- get_data(tracer)

    expect_identical(data$received_namespace, data$actual_namespace)
})


test_that("is_attached correctly identifies if package is attached", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_load = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            key <- paste0("load","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_attach = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            key <- paste0("attach","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_detach = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            key <- paste0("detach","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_unload = function(tracer, callback, state, application, package) {
            data <- get_data(tracer)
            key <- paste0("unload","_", get_name(package))
            data[[key]] <- is_attached(package)
        }
    )

    trace_code(tracer, {
        library(MASS)
        ## accessing symbol from Matrix will load it but not attach it.
        Matrix::tril
        unloadNamespace("MASS")
        unloadNamespace("Matrix")
    })

    data <- get_data(tracer)

    expect_identical(data$load_MASS, FALSE)
    expect_identical(data$attach_MASS, TRUE)
    expect_identical(data$detach_MASS, TRUE)
    expect_identical(data$unload_MASS, FALSE)

    expect_identical(data$load_Matrix, FALSE)
    expect_identical(data$unload_Matrix, FALSE)
})

test_that("get_functions correctly returns all package functions", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_load = function(tracer, callback, state, application, package) {
            package_name <- get_name(package)
            function_names <- names(get_functions(package))
            data <- get_data(tracer)
            data[[package_name]] <- function_names
        },
        package_attach = function(tracer, callback, state, application, package) {
            package_name <- get_name(package)
            function_names <- names(get_functions(package))
            data <- get_data(tracer)
            data[[package_name]] <- function_names
        }
    )

    trace_code(tracer, {
        library(MASS)
        ## accessing symbol from Matrix will load it but not attach it.
        Matrix::tril

        unloadNamespace("MASS")
        unloadNamespace("Matrix")
    })

    data <- get_data(tracer)
    received_mass_funs <- sort(data[["MASS"]])
    received_matrix_funs <- sort(data[["Matrix"]])

    get_function_bindings <- function(ns) {
        fun_names <- ls(envir = ns, all.names = TRUE, sorted = TRUE)
        fun_names <- Filter(function(name) is_closure(get(name, envir = ns)), fun_names)
    }

    library("MASS")
    ns <- getNamespace("MASS")
    expected_mass_funs <- get_function_bindings(ns)

    library("Matrix")
    ns <- getNamespace("Matrix")
    expected_matrix_funs <- get_function_bindings(ns)

    expect_length(received_mass_funs, length(expected_mass_funs))
    expect_length(received_matrix_funs, length(expected_matrix_funs))

    expect_true(all(received_mass_funs == expected_mass_funs))
    expect_true(all(received_matrix_funs == expected_matrix_funs))

    unloadNamespace("MASS")
    unloadNamespace("Matrix")
})

test_that("get_function_count correctly returns package function count", {

    tracer <- create_tracer(
        tracing_entry = function(tracer, callback, state, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_load = function(tracer, callback, state, application, package) {
            package_name <- get_name(package)
            function_count <- get_function_count(package)
            data <- get_data(tracer)
            data[[package_name]] <- function_count
        },
        package_attach = function(tracer, callback, state, application, package) {
            package_name <- get_name(package)
            function_count <- get_function_count(package)
            data <- get_data(tracer)
            data[[package_name]] <- function_count
        }
    )

    trace_code(tracer, {
        library(MASS)
        ## accessing symbol from Matrix will load it but not attach it.
        Matrix::tril

        unloadNamespace("MASS")
        unloadNamespace("Matrix")
    })

    data <- get_data(tracer)
    received_mass_count <- data[["MASS"]]
    received_matrix_count <- data[["Matrix"]]

    get_function_bindings <- function(ns) {
        fun_names <- ls(envir = ns, all.names = TRUE, sorted = TRUE)
        fun_names <- Filter(function(name) is_closure(get(name, envir = ns)), fun_names)
    }

    library("MASS")
    ns <- getNamespace("MASS")
    expected_mass_count <- length(get_function_bindings(ns))

    library("Matrix")
    ns <- getNamespace("Matrix")
    expected_matrix_count <- length(get_function_bindings(ns))

    expect_equal(received_mass_count, expected_mass_count)
    expect_equal(received_matrix_count, expected_matrix_count)

    unloadNamespace("MASS")
    unloadNamespace("Matrix")
})
