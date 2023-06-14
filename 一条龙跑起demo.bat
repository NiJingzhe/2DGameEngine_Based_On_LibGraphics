@echo off
:CONFIG
if EXIST .\build\MakeFile (
	echo CMake配置已经完成，不需要再次配置，上次配置使用MinGW工具链
	PAUSE
	goto BUILD
)
if EXIST .\build\*.sln (
	echo CMake配置已经完成，不需要再次配置，上次配置使用Visual Studio 17 2022工具链
	PAUSE
	goto BUILD
)
echo 构建build目录
mkdir build
echo 进入build目录
cd build

set /p config_type=请输入希望使用的工具链：【1】MinGW 【2】Visual Studio 17 2022 (必填，否则脚本直接退出)：
	
IF %config_type% == 2 (
	echo 开始CMake配置，使用MSVC && cmake .. -G "Visual Studio 17 2022" -A Win32
) ELSE ( 
	IF %config_type% == 1 (
		echo 开始CMake配置，使用MinGW && cmake .. -G "MinGW Makefiles"
	)
)


SET GENERATE_ERRORLEVEL=!ERRORLEVEL! 
IF NOT "!GENERATE_ERRORLEVEL!"=="0" (
	echo CMake配置成功！
	PAUSE
	goto BUILD
) ELSE (
	PAUSE
	EXIT /B !GENERATE_ERRORLEVEL!
)

:BUILD
cd build
if NOT EXIST Makefile (
	if NOT EXIST *.sln (
		echo 请首先运行config.bat进行CMake配置！
		PAUSE
		goto CONFIG
	) else (
		echo 使用Visual Studio工具链配置，请直接打开build文件夹中的解决方案^(^.sln^)实施构建
		PAUSE
		EXIT
	)
)	

set /p thread=你想用几条线程编译该项目？
echo 开始编译项目
make install -j%thread%
PAUSE
set /p run=是否要运行项目？^(y^/n^)^: 
if %run% == y (
	goto RUN
) else (
	EXIT
)

:RUN
cd ..\test_env
if NOT EXIST CyberRunner.exe (
	echo 请首先运行build.bat进行项目构建！
	PAUSE
	goto BUILD
)

.\run.bat
