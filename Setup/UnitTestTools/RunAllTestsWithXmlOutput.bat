@echo off
for /f %%a IN ('dir /b *Test.Exe') do call testRunner.bat %%a
