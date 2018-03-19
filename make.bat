echo off
echo ------ tests ------



REM echo building jasmine_example.cpp ...
REM g++ -c test/jasmine_example.cpp -o jasmine_example.o -std=c++17 -fconcepts -Wno-attributes

REM echo jasmine_example.exe
REM g++ -o jasmine_example.exe jasmine_example.o 

REM echo.
REM echo --- jasmine_example.exe ---
REM jasmine_example.exe




rem echo building test_regexes.cpp ...
rem g++ -c test/test_regexes.cpp -o test_regexes.o -std=c++17 -fconcepts -Wno-attributes

rem echo test_has_element.exe
rem g++ -o test_regexes.exe test_regexes.o 

rem echo.
rem echo --- test_regexes.exe ---
rem test_regexes.exe




rem echo building test_has_element.cpp ...
rem g++ -c test/test_has_element.cpp -o test_has_element.o -std=c++17 -fconcepts -Wno-attributes

rem echo test_has_element.exe
rem g++ -o test_has_element.exe test_has_element.o 

rem echo.
rem echo --- test_has_element.exe ---
rem test_has_element.exe




rem echo building test_extract_lexems.cpp ...
rem g++ -c test/test_extract_lexems.cpp -o test_extract_lexems.o -std=c++17 -fconcepts -Wno-attributes

rem echo test_extract_lexems.exe
rem g++ -o test_extract_lexems.exe test_extract_lexems.o 

rem echo.
rem echo --- test_extract_lexems.exe ---
rem test_extract_lexems.exe



rem echo building test_extract_element.cpp ...
rem g++ -c test/test_extract_element.cpp -o test_extract_element.o -std=c++17 -fconcepts -Wno-attributes

rem echo test_extract_element.exe
rem g++ -o test_extract_element.exe test_extract_element.o 

rem echo.
rem echo --- test_extract_element.exe ---
rem test_extract_element.exe



echo building thodd.cpp ...
g++ -c src/thodd.cpp -o thodd.o -std=c++17 -fconcepts -Wno-attributes -fno-diagnostics-show-caret -fno-diagnostics-color -ftemplate-backtrace-limit=0 2> compile.log

echo thodd.exe
g++ -o thodd.exe thodd.o 

echo.
echo --- thodd.exe ---
thodd.exe


echo.
echo --- clean build files ---
rem rm test_regexes.exe
rem rm test_has_element.exe
rem rm test_extract_lexems.exe
REM rm jasmine_example.exe
rem rm test_extract_element.exe


rem rm test_regexes.o
rem rm test_has_element.o
rem rm test_extract_lexems.o
REM rm jasmine_example.o
rem rm test_extract_element.o
rm thodd.o
            