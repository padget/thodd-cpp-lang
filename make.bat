echo off
set INCLUDES_DIR="-IF:/projects/"
set INCLUDES_ARGS="%INCLUDES_DIR%thodd-cpp-range/include %INCLUDES_DIR%thodd-cpp-container/include %INCLUDES_DIR%thodd-cpp-dsl/include %INCLUDES_DIR%thodd-cpp-optional/include %INCLUDES_DIR%thodd-cpp-functional/include %INCLUDES_DIR%include %INCLUDES_DIR%thodd-cpp-sequence/include %INCLUDES_DIR%thodd-cpp-tests/include %INCLUDES_DIR%thodd-cpp-tuple/include"

echo building regexes.cpp ...
g++ -c src/regexes.cpp -o regexes.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes

echo ------ tests ------
echo building test_has_element.cpp ...
g++ -c test/test_has_element.cpp -o test_has_element.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes

echo test_has_element.exe
g++ -o test_has_element.exe regexes.o test_has_element.o 

echo --- test_has_element.exe ---
test_has_element.exe

rem echo thodd.cpp
rem g++ -g -c src/thodd.cpp -o main.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes
rem echo main.exe
rem g++ -g -o main.exe main.o build_context.o regexes.o


            