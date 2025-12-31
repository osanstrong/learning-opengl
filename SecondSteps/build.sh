# Configure, then build, then if successful run
cmake --log-level=VERBOSE -B build
if cmake --build build; then
    build/main
fi