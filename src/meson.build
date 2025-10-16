rootAppInclude = include_directories('.')

# Top-level library

libcli_sources = [
    'cmd.c',
    'cmdln.c'
]

psapi_cflags = [ ]
link_args = []

libcli_c_args = []

cli_shared_lib = library(
    'cli',                # Name of the library (e.g., libmylib.so on Linux)
    libcli_sources,       # Source files for the library
    version : '1.0.0',    # Optional: specify library version (for shared libraries)
    soversion : '1',      # Optional: specify soname version (for shared libraries)
    install : false       # Optional: install the library
)

# If other targets within the same project need to link against this library,
# you can declare a dependency for them.
# cli_dep = declare_dependency(
#    link_with : cli_shared_lib,
#    include_directories : include_directories(rootAppInclude) # Assuming headers are in the current directory
#)

#libclimain_exe = executable(
#    'libcli',
#    'main.c',
#    c_args: libcli_c_args,
#    dependencies: cli_dep,
#    install: true,
#)
