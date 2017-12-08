call :SafeRMDIR "x64"
call :SafeRMDIR "Debug"
call :SafeRMDIR "TSpring\x64"
exit /b
:SafeDEL
IF EXIST %~1 (
	DEL %~1
)
exit /b
:SafeRMDIR
IF EXIST %~1 (
	RMDIR /S /Q %~1
)
exit /b
