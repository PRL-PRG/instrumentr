
test_that("get_name returns correct package name", {

    tracer <- create_tracer(
        tracing_initialization_callback = function(tracer, callback, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach_callback = function(tracer, callback, application, package) {
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
        tracing_initialization_callback = function(tracer, callback, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach_callback = function(tracer, callback, application, package) {
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
        tracing_initialization_callback = function(tracer, callback, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_attach_callback = function(tracer, callback, application, package) {
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
        tracing_initialization_callback = function(tracer, callback, application) {
            set_data(tracer, new.env(parent = emptyenv()))
        },
        package_load_callback = function(tracer, callback, application, package) {
            data <- get_data(tracer)
            key <- paste0("load","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_attach_callback = function(tracer, callback, application, package) {
            data <- get_data(tracer)
            key <- paste0("attach","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_detach_callback = function(tracer, callback, application, package) {
            data <- get_data(tracer)
            key <- paste0("detach","_", get_name(package))
            data[[key]] <- is_attached(package)
        },
        package_unload_callback = function(tracer, callback, application, package) {
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

