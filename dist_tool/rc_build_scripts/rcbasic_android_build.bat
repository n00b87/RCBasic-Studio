BASEDIR=%~dp0

cd %BASEDIR%

echo "ANDROID BUILD DATA: %1 %2 %3 %4 %5 %6 %7"


echo "RCBASIC_HOME = $RCBASIC_HOME"
set PATH=%RCBASIC_HOME%\bin;%PATH%


echo "VERSION "
rcbasic4 --version
echo "..."

rem #export JAVA_HOME=/usr/bin

set PROJECT_DIR=%1
set BUILD_DIR=%CD%

echo PROJECT_DIR=%PROJECT_DIR%
echo BUILD_DIR=%BUILD_DIR%

echo "___________________"

if defined ANDROID_HOME ( echo "ANDROID_SDK found" ) else ( echo "ERROR: Android SDK not found" && GOTO :EXIT_SCRIPT )

echo "YOLO"


rem NOTE: On windows it will be set in the distribute dialog
rem set PROJECT_NAME=$( cat "$PROJECT_DIR/rcbasic_studio_dist_cmd.txt" | head -n1 | cut -d'=' -f3 | cut -d'"' -f2 )

echo "TEST::: Project[%PROJECT_NAME%]"

rem These variables will be set by the process that calls this script
rem set RC_KEY_STORE=
rem set RC_KEY_STORE_PASS=
rem set RC_ALIAS=
rem set RC_ALIAS_PASS=
rem set APP_ID=org.rcbasic.app
rem set RCBASIC_HOME=

ren Setting up Environment Variables that android sdk needs

rem set ANDROID_HOME=%~dp0android_sdk
rem set ANDROID_SDK_ROOT=%ANDROID_HOME%
rem set ANDROID_HOME=C:\Users\omega\Desktop\ISO\SDL2-2.0.14\android_sdk
rem set ANDROID_HOME=C:\android_sdk

rem JAVA_HOME may not need to be set in linux as long as java is installed
rem export JAVA_HOME=%~dp0openjdk-11
set ANDROID_SDK_ROOT=%ANDROID_HOME%
set ANDROID_NDK_HOME=%ANDROID_HOME%\ndk\27.0.12077973
echo RC_DEBUG_ENVIRONMENT SDK_ROOT=%ANDROID_SDK_ROOT%
set PATH=%ANDROID_HOME%\cmdline-tools\bin;%ANDROID_NDK_HOME%;%JAVA_HOME%\bin:%PATH%

rem # making sure the tools for the target API are installed

set DEFAULT_API_VERSION=36

rem # API_VERSION=$1
set API_VERSION=36

set MIN_API_VERSION=21

set BUILD_TOOLS_VERSION=36

if defined API_VERSION ( echo "API_VERSION Defined" ) else ( echo "API_VERSION will be set to " %DEFAULT_API_VERSION% )


set API_VERSION=%API_VERSION: =%
echo API_VERSION is set to "$API_VERSION"


rem # build gradle
set current_dir=%CD%
cd "%BUILD_DIR%\scripts"
echo IN DIR:%CD%

rem # echo CMD = rcbasic gen_app_settings --APP_NAME "%APP_NAME%" --ORIENTATION "%ORIENTATION%" --ICON "%ICON%" --API_VERSION %API_VERSION% --MIN_API_VERSION %MIN_API_VERSION% --APP_ID %APP_ID% --KEY_STORE "%RC_KEY_STORE%" --KEY_STORE_PASS "%RC_KEY_STORE_PASS%" --ALIAS "%RC_ALIAS%" --ALIAS_PASS "%RC_ALIAS_PASS%" 
set release_setting=--DEBUG

if  "%ANDROID_RELEASE%" == "true" (
set release_setting=--RELEASE
echo "INFO: RELEASE BUILD"
) else (
echo "INFO: NO RELEASE:[%ANDROID_RELEASE%]"
)

echo "ANDROID_APP_ID=[%ANDROID_RELEASE%]"

echo release_setting = %release_setting%
rcbasic4 gen_app_settings --APP_NAME "%PROJECT_NAME%" --ORIENTATION %ANDROID_ORIENTATION% --ICON "%ICON%" --API_VERSION %API_VERSION% --MIN_API_VERSION %MIN_API_VERSION% --APP_ID %ANDROID_APP_ID% --KEY_STORE "%ANDROID_KEYSTORE%" --KEY_STORE_PASS %ANDROID_KEYSTORE_PASS% --ALIAS %ANDROID_ALIAS% --ALIAS_PASS %ANDROID_ALIAS_PASS% %release_setting%

cd "%current_dir%"




if exist "%BUILD_DIR%\android-project\app\src\main\assets" (
rmdir /S /Q "%BUILD_DIR%\android-project\app\src\main\assets"
)

pushd "%BUILD_DIR%\android-project\app\src\main"
mkdir assets
popd
xcopy "%PROJECT_DIR%" "%BUILD_DIR%\android-project\app\src\main\assets" /E /I /H /Y

cd "%BUILD_DIR%\android-project"

echo RC_ANDROID BUILD: DEBUG=%ANDROID_DEBUG%
echo RC_ANDROID BUILD: RELEASE=%ANDROID_RELEASE%

set assemble_tgt=

if "%ANDROID_DEBUG%" EQU "true" (
set assemble_tgt=assembleDebug
)

if "%ANDROID_RELEASE%" EQU "true" (
set assemble_tgt=assembleRelease
)

rem # if "%RC_ANDROID_DEBUG_INSTALL%"=="1" ( .\gradlew uninstallDebug && .\gradlew installDebug )

if exist "%BUILD_DIR%\android-project\app/build\outputs" (
rmdir /S /Q "%BUILD_DIR%\android-project\app\build\outputs"
)

echo "starting gradle build: %assemble_tgt%"

gradlew %assemble_tgt%

if exist "%BUILD_DIR%\android-project\app\build\outputs" (

if "%assemble_tgt%" EQU "assembleRelease" (
pushd "%OUTPUT_DIR%"
echo "IN_OUT_DIR: %CD%"

for %%A in ("%CD%\__") do for %%B in ("%%~dpA.") do set base_filename=%%~nxB

set base_folder=%base_filename: =%_ANDROID

cd "%base_folder%"
set apk_filename=%base_filename: =%.apk
copy "%BUILD_DIR%\android-project\app\build\outputs\apk\release\app-release.apk" %apk_filename%
popd
) else (
pushd "%OUTPUT_DIR%"
for %%A in ("%CD%\__") do for %%B in ("%%~dpA.") do set base_filename=%%~nxB
set base_folder=%base_filename: =%_ANDROID
cd "%base_folder%"
set apk_filename=%base_filename: =%.apk
copy "%BUILD_DIR%\android-project\app\build\outputs\apk\debug\app-debug.apk" %apk_filename%
popd
)

)

rem # sdkmanager --sdk_root=%ANDROID_HOME% "platform-tools" "platforms;android-28" "build-tools;28.0.3" "system-images;android-28;google_apis;x86_64" "ndk;21.4.7075529"


:EXIT_SCRIPT
echo "End build script"

cd "%PROJECT_DIR%"
