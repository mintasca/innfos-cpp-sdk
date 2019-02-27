@echo off
SET var=%cd%
ECHO %var%
setx ACTUATOR_TOOL_PATH %var%
pause