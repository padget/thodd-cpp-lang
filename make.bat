echo off
echo ------ tests ------



echo building test_regexes.cpp ...
g++ -c test/test_regexes.cpp -o test_regexes.o -std=c++17 -fconcepts -Wno-attributes

echo test_has_element.exe
g++ -o test_regexes.exe test_regexes.o 

echo.
echo --- test_regexes.exe ---
test_regexes.exe



echo building test_has_element.cpp ...
g++ -c test/test_has_element.cpp -o test_has_element.o -std=c++17 -fconcepts -Wno-attributes

echo test_has_element.exe
g++ -o test_has_element.exe test_has_element.o 

echo.
echo --- test_has_element.exe ---
test_has_element.exe



echo building test_extract_lexems.cpp ...
g++ -c test/test_extract_lexems.cpp -o test_extract_lexems.o -std=c++17 -fconcepts -Wno-attributes

echo test_extract_lexems.exe
g++ -o test_extract_lexems.exe test_extract_lexems.o 

echo.
echo --- test_extract_lexems.exe ---
test_extract_lexems.exe





echo.
echo --- clean build files ---
rm test_regexes.exe
rm test_has_element.exe
rm test_extract_lexems.exe

rm test_regexes.o
rm test_has_element.o
rm test_extract_lexems.o

            