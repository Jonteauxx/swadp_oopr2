# =============================================================================
# tests/CMakeLists.txt - Unit-test target voor Google Test.
# =============================================================================
# Tests draaien automatisch via:
#   cd build && ctest --output-on-failure
# of bij elke deploy via scripts/pi-update.sh.
#
# Belangrijk: we linken hier alleen domain/infra-source die GEEN Qt
# nodig heeft. HallSensor.cpp / MainWindow etc blijven uit de test-build.
# Test-files gebruiken indien nodig hun eigen stub-subklasse (zie Sensor_test.cpp).
# =============================================================================

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_tests
    MockGpio_test.cpp
    Sensor_test.cpp

    # Source files (zonder Qt-afhankelijkheid)
    ../src/infra/MockGpio.cpp
    ../src/domain/Sensor.cpp
)

target_include_directories(unit_tests PRIVATE
    ../src
    ../src/infra
    ../src/domain
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

# Ontdek alle TEST(...) macros automatisch zodat ctest ze ziet.
gtest_discover_tests(unit_tests)
