@echo off
cd /d %~dp0
SET var=%cd%
ECHO %var%
setx ACTUATOR_TOOL_PATH %var%
netsh advfirewall firewall add rule name="Actuator" protocol=UDP dir=in action=allow
pause