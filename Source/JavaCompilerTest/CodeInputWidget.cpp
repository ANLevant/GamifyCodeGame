// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeInputWidget.h"
#include <string>
#include <fstream>
#include "../jdk1.8.0_211/include/jni.h"
#include<iostream>
#include <stdio.h>
#include<dlfcn.h>

void UCodeInputWidget::compileCode(FText codeToCompile)
{
    JavaVM *vm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    

    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 0;
    vm_args.ignoreUnrecognized = 1;

    //Construct JVM
    int res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);

    UE_LOG(LogTemp, Warning, TEXT("JVM created successfuly with result code: %s"), res);
    FString printable =  codeToCompile.ToString();
    FString filePath = FString("/home/levant/test.java");
    FFileHelper::SaveStringToFile(printable, *filePath);

    system("javac /home/levant/test.java");

    UE_LOG(LogTemp, Warning, TEXT("Class compiled successfuly"));

    try {
        jclass clazz = env->FindClass("java/lang/String");

        UE_LOG(LogTemp, Warning, TEXT("Class loaded successfuly"));

        jstring jstr = env->NewStringUTF("Hello");

        // Get the method that you want to call
        jmethodID to_lower = env->GetMethodID(clazz, "toLowerCase", "()Ljava/lang/String;");

        // Call the method on the object
        jobject result = env->CallObjectMethod(jstr, to_lower);

        jstring resultString = (jstring) result;

        // Get a C-style string
        const char* str = env->GetStringUTFChars(resultString, NULL);

        env->ReleaseStringUTFChars(resultString, str);

        UE_LOG(LogTemp, Warning, TEXT("Class method invoked successfuly with result: %s"), str);
    }
    catch(...){
        UE_LOG(LogTemp, Warning, TEXT("Class couldn't be loaded"));
    }

    vm->DestroyJavaVM();
}


