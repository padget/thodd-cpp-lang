echo off
echo ------ tests ------

echo building test_regexes.cpp ...
g++ -c test/test_regexes.cpp -o test_regexes.o -std=c++17 -fconcepts -Wno-attributes

echo test_has_element.exe
g++ -o test_regexes.exe regexes.o test_regexes.o 

echo.
echo --- test_regexes.exe ---
test_regexes.exe

echo building test_has_element.cpp ...
g++ -c test/test_has_element.cpp -o test_has_element.o -std=c++17 -fconcepts -Wno-attributes

echo test_has_element.exe
g++ -o test_has_element.exe regexes.o test_has_element.o 

echo.
echo --- test_has_element.exe ---
test_has_element.exe




            