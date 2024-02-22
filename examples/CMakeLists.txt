find_package(Threads REQUIRED)

set(EXAMPLE1_SOURCES
    examples/aces_encrypt_decrypt.c
)

add_executable(run_example1 ${EXAMPLE1_SOURCES})
target_link_libraries(run_example1 PRIVATE
    CAces
    Threads::Threads
)
