cmake_minimum_required(VERSION 3.16)
project(gebouw VERSION 0.1 LANGUAGES CXX)

# === C++ standaard ===
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Qt automatische tools ===
# AUTOMOC: genereert moc-files voor Qt signals/slots
# AUTOUIC: compileert .ui bestanden naar header
# AUTORCC: verwerkt .qrc resource-bundels
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# === Qt6 zoeken ===
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# === pigpio zoeken (alleen op de Pi aanwezig) ===
# We gebruiken pigpiod_if2: praat via socket met de pigpiod daemon,
# zodat we niet als root hoeven te draaien. Op Windows ontbreekt
# pigpio simpelweg - dan compileren we tegen de mock.
find_library(PIGPIOD_IF2_LIB pigpiod_if2)
if(PIGPIOD_IF2_LIB)
    message(STATUS "pigpiod_if2 gevonden: ${PIGPIOD_IF2_LIB}")
    set(HAS_PIGPIO TRUE)
else()
    message(STATUS "pigpiod_if2 niet gevonden - alleen MockGpio beschikbaar")
    set(HAS_PIGPIO FALSE)
endif()

# === Bron-files ===
# Worden later uitgebreid met domain/, infra/, app/, ui/
set(GEBOUW_SOURCES
    src/main.cpp
)

add_executable(gebouw ${GEBOUW_SOURCES})

target_include_directories(gebouw PRIVATE src)
target_link_libraries(gebouw PRIVATE Qt6::Core Qt6::Widgets)

if(HAS_PIGPIO)
    target_link_libraries(gebouw PRIVATE ${PIGPIOD_IF2_LIB})
    target_compile_definitions(gebouw PRIVATE HAS_PIGPIO=1)
endif()

# === Unit-tests (later) ===
# enable_testing()
# add_subdirectory(tests)
