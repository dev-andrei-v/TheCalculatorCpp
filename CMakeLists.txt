cmake_minimum_required(VERSION 3.14)
project(TheCalculator VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Shared core library (pipeline + engine) ---

add_library(calc_core STATIC
    src/pipeline/Tokenizer.cpp
    src/pipeline/Parser.cpp
    src/pipeline/Evaluator.cpp
    src/engine/Calculator.cpp
    src/engine/VariableStore.cpp
)
target_include_directories(calc_core PUBLIC src)

# --- CLI executable ---

add_executable(TheCalculator
    src/main.cpp
    src/cli/CLI.cpp
    src/cli/CommandRouter.cpp
    src/cli/FileIO.cpp
)
target_link_libraries(TheCalculator PRIVATE calc_core)

# --- GUI executable (requires GTK4) ---

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GTK4 gtk4)
endif()

if(GTK4_FOUND)
    add_executable(TheCalculatorGUI
        src/gui/main_gui.cpp
        src/gui/CalculatorWindow.cpp
    )
    target_link_libraries(TheCalculatorGUI PRIVATE calc_core ${GTK4_LIBRARIES})
    target_include_directories(TheCalculatorGUI PRIVATE ${GTK4_INCLUDE_DIRS})
    target_compile_options(TheCalculatorGUI PRIVATE ${GTK4_CFLAGS_OTHER})
    message(STATUS "GTK4 found — building GUI target")
else()
    message(STATUS "GTK4 not found — skipping GUI target (install libgtk-4-dev)")
endif()
