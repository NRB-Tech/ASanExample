#include <jni.h>
#include <stdlib.h>

void JNICALL Java_io_nrbtech_asanexample_Test_test(JNIEnv *jenv, jclass jcls) {
    void * ptr = malloc(10);
    free(ptr);
    free(ptr);
}