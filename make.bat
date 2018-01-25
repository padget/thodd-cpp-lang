echo off
set INCLUDES_DIR="-IF:/projects/"
set INCLUDES_ARGS="%INCLUDES_DIR%thodd-cpp-range/include %INCLUDES_DIR%thodd-cpp-container/include %INCLUDES_DIR%thodd-cpp-dsl/include %INCLUDES_DIR%thodd-cpp-optional/include %INCLUDES_DIR%thodd-cpp-functional/include %INCLUDES_DIR%include %INCLUDES_DIR%thodd-cpp-sequence/include %INCLUDES_DIR%thodd-cpp-tests/include %INCLUDES_DIR%thodd-cpp-tuple/include"

echo build_context.cpp
g++ -g -c src/build_context.cpp -o build_context.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes
echo regexes.cpp
g++ -g -c src/regexes.cpp -o regexes.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes

echo ------ tests ------
echo test_extract_element.cpp
g++ -g -c test/test_extract_element.cpp -o test_extract_element.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes
echo test_extract_element.exe
g++ -g -o test_extract_element.exe regexes.o test_extract_element.o 
echo --- test_extract_element.exe ---
test_extract_element.exe

echo thodd.cpp
g++ -g -c src/thodd.cpp -o main.o -std=c++17 -fconcepts %INCLUDES_ARGS% -Wno-attributes
echo main.exe
g++ -g -o main.exe main.o build_context.o regexes.o


            