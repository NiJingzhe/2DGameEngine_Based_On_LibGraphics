@echo off
:CONFIG
if EXIST .\build\MakeFile (
	echo CMake�����Ѿ���ɣ�����Ҫ�ٴ����ã��ϴ�����ʹ��MinGW������
	PAUSE
	goto BUILD
)
if EXIST .\build\*.sln (
	echo CMake�����Ѿ���ɣ�����Ҫ�ٴ����ã��ϴ�����ʹ��Visual Studio 17 2022������
	PAUSE
	goto BUILD
)
echo ����buildĿ¼
mkdir build
echo ����buildĿ¼
cd build

set /p config_type=������ϣ��ʹ�õĹ���������1��MinGW ��2��Visual Studio 17 2022 (�������ű�ֱ���˳�)��
	
IF %config_type% == 2 (
	echo ��ʼCMake���ã�ʹ��MSVC && cmake .. -G "Visual Studio 17 2022" -A Win32
) ELSE ( 
	IF %config_type% == 1 (
		echo ��ʼCMake���ã�ʹ��MinGW && cmake .. -G "MinGW Makefiles"
	)
)


SET GENERATE_ERRORLEVEL=!ERRORLEVEL! 
IF NOT "!GENERATE_ERRORLEVEL!"=="0" (
	echo CMake���óɹ���
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
		echo ����������config.bat����CMake���ã�
		PAUSE
		goto CONFIG
	) else (
		echo ʹ��Visual Studio���������ã���ֱ�Ӵ�build�ļ����еĽ������^(^.sln^)ʵʩ����
		PAUSE
		EXIT
	)
)	

set /p thread=�����ü����̱߳������Ŀ��
echo ��ʼ������Ŀ
make install -j%thread%
PAUSE
set /p run=�Ƿ�Ҫ������Ŀ��^(y^/n^)^: 
if %run% == y (
	goto RUN
) else (
	EXIT
)

:RUN
cd ..\test_env
if NOT EXIST CyberRunner.exe (
	echo ����������build.bat������Ŀ������
	PAUSE
	goto BUILD
)

.\run.bat
