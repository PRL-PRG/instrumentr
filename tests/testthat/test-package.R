
test_that("get_name returns correct package name", {

    tracer <- create_tracer(
        package_attach = function(tracer, callback, state, application, environment) {
            print('assinging')
            insert(state, "name", get_name(environment))
            print('here')
        }
    )

    result <- trace_code(tracer, {
        library(MASS)
        print("unloading")
        unloadNamespace("MASS")
        print("done unloading")
    })

    expect_identical("MASS", result$output$name)
})

##test_that("get_namespace returns correct package namespace", {
##
##    tracer <- create_tracer(
##        package_attach = function(tracer, callback, state, application, environment) {
##            insert(state, "received_namespace", get_object_details(namespace)$address)
##            insert(state, "actual_namespace", get_object_details(getNamespace("MASS"))$address)
##        }
##    )
##
##    result <- trace_code(tracer, {
##        library(MASS)
##        unloadNamespace("MASS")
##    })
##
##    expect_identical(result$output$received_namespace, result$output$actual_namespace)
##})


##test_that("get_functions correctly returns all package functions", {
##
##    tracer <- create_tracer(
##        tracing_entry = function(tracer, callback, state, application) {
##        },
##        package_load = function(tracer, callback, state, application, environment) {
##            package_name <- get_name(environment)
##            function_names <- names(get_functions(environment))
##            instrumentr::insert(state, package_name, function_names)
##        },
##        package_attach = function(tracer, callback, state, application, environment) {
##            package_name <- get_name(environment)
##            function_names <- names(get_functions(environment))
##            instrumentr::insert(state, package_name, function_names)
##        }
##    )
##
##    result <- trace_code(tracer, {
##        library(MASS)
##        ## accessing symbol from Matrix will load it but not attach it.
##        Matrix::tril
##
##        print('here')
##        unloadNamespace("MASS")
##        unloadNamespace("Matrix")
##        print('unloaded')
##    })
##
##    received_mass_funs <- sort(result$output[["MASS"]])
##    received_matrix_funs <- sort(result$output[["Matrix"]])
##
##    get_function_bindings <- function(ns) {
##        fun_names <- ls(envir = ns, all.names = TRUE, sorted = TRUE)
##        fun_names <- Filter(function(name) is_closure(get(name, envir = ns)), fun_names)
##    }
##
##    print('here2')
##    library("MASS")
##    ns <- getNamespace("MASS")
##    expected_mass_funs <- get_function_bindings(ns)
##    print('here3')
##    library("Matrix")
##    ns <- getNamespace("Matrix")
##    expected_matrix_funs <- get_function_bindings(ns)
##    print('here4')
##    expect_length(received_mass_funs, length(expected_mass_funs))
##    expect_length(received_matrix_funs, length(expected_matrix_funs))
##    print('here5')
##    expect_true(all(received_mass_funs == expected_mass_funs))
##    expect_true(all(received_matrix_funs == expected_matrix_funs))
##
##    unloadNamespace("MASS")
##    unloadNamespace("Matrix")
##})
##
##test_that("get_function_count correctly returns package function count", {
##
##    tracer <- create_tracer(
##        tracing_entry = function(tracer, callback, state, application) {
##            print("tracing_entry")
##        },
##        package_load = function(tracer, callback, state, application, environment) {
##            print("package_load begin")
##            package_name <- get_name(environment)
##            function_count <- get_function_count(environment)
##            instrumentr::insert(state, package_name, function_count)
##            print("package_load end")
##        },
##        package_attach = function(tracer, callback, state, application, environment) {
##            print("package_attach begin")
##            package_name <- get_name(environment)
##            function_count <- get_function_count(environment)
##            instrumentr::insert(state, package_name, function_count)
##            print("package_attach end")
##        }
##    )
##
##    result <- trace_code(tracer, {
##        library(MASS)
##        ## accessing symbol from Matrix will load it but not attach it.
##        Matrix::tril
##
##        print("here .. 1")
##        unloadNamespace("MASS")
##        print("here .. 2")
##        unloadNamespace("Matrix")
##        print("here .. 3")
##    })
##
##    print(result)
##
##    print("here also")
##    received_mass_count <- result$output[["MASS"]]
##    received_matrix_count <- result$output[["Matrix"]]
##
##    get_function_bindings <- function(ns) {
##        fun_names <- ls(envir = ns, all.names = TRUE, sorted = TRUE)
##        fun_names <- Filter(function(name) is_closure(get(name, envir = ns)), fun_names)
##    }
##
##    library("MASS")
##    ns <- getNamespace("MASS")
##    expected_mass_count <- length(get_function_bindings(ns))
##
##    library("Matrix")
##    ns <- getNamespace("Matrix")
##    expected_matrix_count <- length(get_function_bindings(ns))
##
##    expect_equal(received_mass_count, expected_mass_count)
##    expect_equal(received_matrix_count, expected_matrix_count)
##
##    unloadNamespace("MASS")
##    unloadNamespace("Matrix")
##})
