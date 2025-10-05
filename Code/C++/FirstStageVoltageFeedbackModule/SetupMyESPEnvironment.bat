echo Setting up ESP-IDF environment...

:: Set the IDF_PATH if not already set
if not defined IDF_PATH (
    set IDF_PATH=C:\Users\USER1\esp\v5.4\esp-idf
    echo IDF_PATH set to %IDF_PATH%
)

:: Set the IDF_TOOLS_PATH if not already set
if not defined IDF_TOOLS_PATH (
    set IDF_TOOLS_PATH=C:\Users\USER1\esp\v5.4\tools
    echo IDF_TOOLS_PATH set to %IDF_TOOLS_PATH%
)

:: Add ESP-IDF tools to PATH
set PATH=%IDF_PATH%\tools;%IDF_TOOLS_PATH%\bin;%PATH%
echo PATH updated to %PATH%

:: Set PYTHONPATH
set PYTHONPATH=%IDF_PATH%\python_env\idf4.4_py3.8_env\Scripts;%PYTHONPATH%
echo PYTHONPATH set to %PYTHONPATH%

:: Call the export script to set up the environment
echo Calling export.bat to set up the environment...
call %IDF_PATH%\export.bat

:: Verify the setup
echo Verifying ESP-IDF setup...
idf.py --version

:: Navigate to the project directory
cd C:\path\to\your\project
echo Changed directory to %CD%

:: Run menuconfig
echo Running idf.py menuconfig...
idf.py menuconfig

:: Build the project
echo Building the project...
idf.py build

:: Flash the project
echo Flashing the project...
idf.py flash

:: Monitor the device
echo Monitoring the device...
idf.py monitor

pause