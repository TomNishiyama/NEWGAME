:: Doxygen生成および表示
@echo off

cd document
"%ProgramFiles%\doxygen\bin\doxygen" .\Doxyfile
start .\html\index.html

exit /b 0
