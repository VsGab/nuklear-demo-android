# nuklear-demo-android
Nuklear + SDL Android demo

## Setup
* Create link to SDL source directory in app/jni - e.g `` ln -s ~/some/dir/SDL2-2.0.10/ SDL``
* Import into Android Studio

## Structure
* Actual demo code in *app/jni/src/nuklear-demo.c*
* Current (Jan 2020) version of *app/jni/src/nuclear.h*
* SDL Java package containing base activity and helpers

## Notes
* Single platform APK size ~600K with unneeded features disabled in SDL's Android.mk <br>
`-DSDL_AUDIO_DISABLED 
	-DSDL_JOYSTICK_DISABLED -DSDL_JOYSTICK_DUMMY 
	-DSDL_HAPTIC_DISABLED -DSDL_RENDER_DISABLED 
	-DSDL_ATOMIC_DISABLED -DSDL_POWER_DISABLED 
	-DSDL_FILESYSTEM_DISABLED -DSDL_THREADS_DISABLED 
	-DSDL_TIMERS_DISABLED -DSDL_FILE_DISABLED 
	-DSDL_LOADSO_DISABLED -DSDL_CPUINFO_DISABLED 
	-DSDL_GAMECONTROLLER_DISABLED`
* CPU usage low due to usage of *SDL_WaitEvent* instead of polling
* Using default font (Proggy Clean) embedded in Nuklear

## Demo
Button, option, text input, wrapped text, styling <br><br>
![Screenshot](/screen.png?raw=true)


## TODO
* APK size could be further reduced - strip JNI interface and Java helpers, investigate further SDL shared library stripping
* Native project not well integrated with IDE - C sources not seen as part of the project, ndk-build created libs dir unused
