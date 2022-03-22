# Address Sanitizer example

When developing for Android, Google have a number of tools available to help debug memory issues in C/C++ code. The approach recommended by Google is to use ASan, however the [ASan documentation](https://developer.android.com/ndk/guides/asan) is currently (March ‘22, Android Studio 2021.1.1, Gradle 7, APIs 27+) missing crucial information to enable developers to debug these issues. This example project demonstrates using ASan to debug a memory issue.

Instructions to modify from a stock Android project with CMake:

1. Run the `sanitize-setup-script` in the root of this project (copy to your own project, change the ndk version if required, and run)
2. In your `CMakeLists.txt`, add the following (replacing `${TARGET}` as required with the name of your target):
   ```cmake
    if(SANITIZE)
        target_compile_options(${TARGET} PUBLIC -fsanitize=address -fno-omit-frame-pointer)
        set_target_properties(${TARGET} PROPERTIES LINK_FLAGS -fsanitize=address)
    endif()
    ```
3. In your app's `build.gradle`, to your `android.buildTypes` closure, add:
   ```groovy
   sanitize {
       initWith debug
       externalNativeBuild {
           cmake {
               arguments "-DANDROID_STL=c++_shared", "-DSANITIZE=TRUE"
           }
       }
   }
   ```
4. Sync Gradle and switch your active variant to "sanitize" in the "Build Variants" tab (bottom left)
5. Debug your App and reproduce the issue. You should see output from address sanitizer (filter by "wrap.sh"). In the stack traces you will see lines like:

       #1 0x5e2cdaf76c  (/data/app/~~lTqqcXu2okmchd_nMeTm7w==/io.nrbtech.asanexample-spc73OiysYUjJ2984-N4kw==/lib/arm64/libasanexample.so+0x76c)

   Where `libasanexample.so` is the name of the compiled C library and `0x76c` represents the offset into the library the stack trace is pointing to.
8. Copy the `symbolizer` script in the root of this project to your own and modify for your C library name. You can then run this script like so:

   ```bash
   % ./symbolizer -a arm64-v8a 0x76c
   Java_io_nrbtech_asanexample_Test_test
   /Users/nick/Documents/Repos/ASanExample/app/src/main/cpp/test.c:7:5
   ```

   The output contains the path to the line of code pointed to by the stack trace, enabling you to find where the issue occurred and figure out the problem. In this case, it points to the line where we free'd the pointer for the second time.