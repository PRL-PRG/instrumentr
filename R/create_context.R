
#' @export
create_context <- function(initializer = function(application) { },
                           finalizer = function(application) { },
                           package_entry_callback = function(application, package) { },
                           package_exit_callback = function(application, package) { },
                           call_entry_callback = function(application, package, func, call) { },
                           call_exit_callback = function(application, package, func, call) { }) {

    is_closure(initializer)
    is_closure(finalizer)
    is_closure(package_entry_callback)
    is_closure(package_exit_callback)
    is_closure(call_entry_callback)
    is_closure(call_exit_callback)

    environment <- sys.frame(sys.nframe())

    .Call(C_context_create_context,
          initializer,
          finalizer,
          package_entry_callback,
          package_exit_callback,
          call_entry_callback,
          call_exit_callback,
          environment)
}
