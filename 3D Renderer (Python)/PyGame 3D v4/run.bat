cd .\Python Code
cls
@ECHO OFF

:START
cls
echo Do you want to:
echo 1: Run
echo 2: Compile .pyc
echo 3: Compile .pyo
echo 4: Profile
echo 5: Exit
set/p "cho=>"
if %cho%==1 goto RUN
if %cho%==2 goto COMPILEPYC
if %cho%==3 goto COMPILEPYO
if %cho%==4 goto PROFILE
if %cho%==5 goto END
echo That is not a valid answer
pause
goto START

:COMPILEPYC
cls
python Functions.py c
copy ".\__pycache__\Py3D.cpython-32.pyc" "..\Compiled Files\Py3D.pyc"
pause
goto START

:COMPILEPYO
cls
python -O Functions.py c
copy ".\__pycache__\Py3D.cpython-32.pyo" "..\Compiled Files\Py3D.pyo"
pause
goto START

:PROFILE
cls
python Functions.py p
pause
goto START

:RUN
cls
echo Which program do you want to run?
echo 1: Basic demo
echo 2: Mesh demo
echo 3: Mouse Movement demo
set/p "cho=>"
if %cho%==1 goto PROGA
if %cho%==2 goto PROGB
if %cho%==3 goto PROGC
echo That is not a valid answer
pause
goto RUN

:PROGA
cls
python game1.py
pause
goto START

:PROGB
cls
python game2.py
pause
goto START

:PROGC
cls
python game3.py
pause
goto START

:END