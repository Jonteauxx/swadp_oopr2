# =============================================================================
# tests/CMakeLists.txt - Unit-test target voor Google Test.
# =============================================================================
# Wordt automatisch meegebouwd door de root CMakeLists. Tests draaien op
# de Pi met:
#   cd build && ctest --output-on-failure
# of zichtbaar tijdens elke deploy via scripts/pi-update.sh.
# =============================================================================

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_tests
    MockGpio_test.cpp
    ../src/infra/MockGpio.cpp
)

target_include_directories(unit_tests PRIVATE
    ../src
    ../src/infra
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Automatisch ontdekken van TEST(...) macros - dan ziet ctest ze.
gtest_discover_tests(unit_tests)
