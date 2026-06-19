cmake_minimum_required(VERSION 3.16)

project(CalculatorProject LANGUAGES CXX)

# Используем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================================
# Библиотека калькулятора
# ======================================================

add_library(calculator_lib

    ../Tokenize/tokenize.cpp

    ../Calculator/parser.cpp
    ../Calculator/calculator.cpp

    ../PolishNotation/polish_notation.cpp
)

# Пути к заголовочным файлам
target_include_directories(calculator_lib PUBLIC

    ../Tokenize
    ../Calculator
    ../PolishNotation
)

# ======================================================
# GoogleTest
# ======================================================

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# ======================================================
# Tokenize tests
# ======================================================

add_executable(tokenize_test
    tokenize_test.cpp
)

target_link_libraries(tokenize_test
    PRIVATE
        calculator_lib
        GTest::gtest_main
)

# ======================================================
# Parser tests
# ======================================================

add_executable(parser_test
    parser_test.cpp
)

target_link_libraries(parser_test
    PRIVATE
        calculator_lib
        GTest::gtest_main
)

# ======================================================
# Calculator tests
# ======================================================

add_executable(calculator_test
    calculator_test.cpp
)

target_link_libraries(calculator_test
    PRIVATE
        calculator_lib
        GTest::gtest_main
)

# ======================================================
# Регистрация тестов
# ======================================================

include(GoogleTest)

gtest_discover_tests(tokenize_test)
gtest_discover_tests(parser_test)
gtest_discover_tests(calculator_test)