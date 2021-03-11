#include <jni.h>
#include <string>

// macros of names of classes
#define MIDDLECLS "lu/uni/jungao/bm11/MiddleCls"
#define DELEGATORCLS "lu/uni/jungao/bm11/NativeDelegator"

// macro of names of methods
#define SENDSMS "sendPseudoSMS"
#define GETIMEI "getImei"
#define NGETIMEI "nativeGetIMEI"

// macro of signatures
#define SIGSENDSMS "(Ljava/lang/String;)V"
#define SIGGETIMEI "()Ljava/lang/String;"
#define SIGNGETIMEI "(Landroid/telephony/TelephonyManager;)Ljava/lang/String;"

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm11_NativeDelegator_sStringFromJNI(
        JNIEnv *env,
        jclass /* cls */) {
    std::string hello = "Hello from C++ static method";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_lu_uni_jungao_bm11_NativeDelegator_iStringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ instance method";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_lu_uni_jungao_bm11_NativeDelegator_nativeSendSMS(JNIEnv *env, jobject /*this*/, jstring msg) {
    jclass MiddleCls = env->FindClass(MIDDLECLS);
    jmethodID sendPSMS = env->GetStaticMethodID(MiddleCls, SENDSMS, SIGSENDSMS);
    env->CallStaticVoidMethod(MiddleCls, sendPSMS, msg);
}

jstring getImei(JNIEnv *env, jobject /*this*/, jobject teleManager) {
    jclass teleManagerClass = env->GetObjectClass(teleManager);
    jmethodID getImei = env->GetMethodID(teleManagerClass, GETIMEI, SIGGETIMEI);
    jstring imei = (jstring) env->CallObjectMethod(teleManager, getImei);
    return imei;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass(DELEGATORCLS);
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {NGETIMEI, SIGNGETIMEI, reinterpret_cast<jstring*>(getImei)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}


