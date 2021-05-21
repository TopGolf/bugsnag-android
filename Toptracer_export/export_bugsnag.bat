@echo off

set bugsnag_android_aar=..\bugsnag-android\build\outputs\aar\bugsnag-android-release.aar
set bugsnag_android_core_aar=..\bugsnag-android-core\build\outputs\aar\bugsnag-android-core-release.aar
set bugsnag_android_ndk_aar=..\bugsnag-android-ndk\build\outputs\aar\bugsnag-android-ndk-release.aar
set bugsnag_android_anr_aar=..\bugsnag-plugin-android-anr\build\outputs\aar\bugsnag-plugin-android-anr-release.aar
set bugsnag_android_ndk_p_aar=..\bugsnag-plugin-android-ndk\build\outputs\aar\bugsnag-plugin-android-ndk-release.aar

set bugsnag_android_jar=bugsnag-android-release.jar
set bugsnag_android_core_jar=bugsnag-android-core-release.jar
set bugsnag_android_anr_jar=bugsnag-plugin-android-anr-release.jar
set bugsnag_android_ndk_jar=bugsnag-plugin-android-ndk-release.jar

set bugsnag_android_ndk_so=libbugsnag-ndk.so
set bugsnag_android_anr_so=libbugsnag-plugin-android-anr.so

set so_source=jni\armeabi-v7a\

set temp_zip=.\temp.zip
set unzippedpath=.\temp\
set class_file=classes.jar

set jlib_jlib_path=.\JavaLibs\
set jlib_so_path=.\JavaLibs\armeabi-v7a\

mkdir %jlib_jlib_path%
mkdir %jlib_so_path%

echo Extracting bugsnag_android_core
copy %bugsnag_android_core_aar% %temp_zip%
powershell -command "Expand-Archive -Force '%temp_zip%' '%unzippedpath%'"
del %temp_zip%
copy %unzippedpath%%class_file% %jlib_jlib_path%%bugsnag_android_core_jar%
del /Q %unzippedpath%

echo Extracting bugsnag_android_anr"
copy %bugsnag_android_anr_aar% %temp_zip%
powershell -command "Expand-Archive -Force '%temp_zip%' '%unzippedpath%'"
del %temp_zip%
copy %unzippedpath%%class_file% %jlib_jlib_path%%bugsnag_android_anr_jar%
copy %unzippedpath%%so_source%%bugsnag_android_anr_so% %jlib_so_path%%bugsnag_android_anr_so%
del /Q %unzippedpath%

echo Extracting bugsnag_android_ndk_plugin
copy %bugsnag_android_ndk_p_aar% %temp_zip%
powershell -command "Expand-Archive -Force '%temp_zip%' '%unzippedpath%'"
del %temp_zip%
copy %unzippedpath%%class_file% %jlib_jlib_path%%bugsnag_android_ndk_jar%
copy %unzippedpath%%so_source%%bugsnag_android_ndk_so% %jlib_so_path%%bugsnag_android_ndk_so%
del /Q %unzippedpath%

pause