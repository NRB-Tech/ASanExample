#!/bin/bash

cd "$(dirname "$0")"

# This script sets up the required directories in app/src/sanitize. It can be run again if you change the ndk version.

ndkversion="24.0.8215888"

# store wrap.sh to variable
read -r -d '' script <<'EOT'
#!/system/bin/sh
HERE=$(cd "$(dirname "$0")" && pwd)

cmd=$1
shift

export ASAN_OPTIONS=log_to_syslog=false,allow_user_segv_handler=1
ASAN_LIB=$(ls "$HERE"/libclang_rt.asan-*-android.so)
if [ -f "$HERE/libc++_shared.so" ]; then
   # Workaround for https://github.com/android-ndk/ndk/issues/988.
   export LD_PRELOAD="$ASAN_LIB $HERE/libc++_shared.so"
else
   export LD_PRELOAD="$ASAN_LIB"
fi

os_version=$(getprop ro.build.version.sdk)

if [ "$os_version" -eq "27" ]; then
 cmd="$cmd -Xrunjdwp:transport=dt_android_adb,suspend=n,server=y -Xcompiler-option --debuggable $@"
elif [ "$os_version" -eq "28" ]; then
 cmd="$cmd -XjdwpProvider:adbconnection -XjdwpOptions:suspend=n,server=y -Xcompiler-option --debuggable $@"
else
 cmd="$cmd -XjdwpProvider:adbconnection -XjdwpOptions:suspend=n,server=y $@"
fi

exec $cmd

EOT

# loop through all architectures and write/copy the required files
archs=( "arm64-v8a" "armeabi-v7a" "x86" "x86_64" )
asanarchs=( "aarch64" "arm" "i686" "x86_64" )

for i in "${!archs[@]}"; do
   mkdir -p app/src/sanitize/{jniLibs,resources/lib}/${archs[$i]}
   cp $ANDROID_HOME/ndk/$ndkversion/toolchains/llvm/prebuilt/*/lib64/clang/*/lib/linux/libclang_rt.asan-${asanarchs[$i]}-android.so app/src/sanitize/jniLibs/${archs[$i]}/
   echo "$script" > app/src/sanitize/resources/lib/${archs[$i]}/wrap.sh
done



