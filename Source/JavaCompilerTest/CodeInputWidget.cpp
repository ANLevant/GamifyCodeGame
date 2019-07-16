// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeInputWidget.h"
#include <string>
#include <fstream>
#include "../jdk1.8.0_211/include/jni.h"
#include<iostream>
#include <stdio.h>
#include<dlfcn.h>


JavaVM *vm;
JNIEnv *env;
JavaVMInitArgs vm_args;
JavaVMOption options[1];
FString relativePath;
FString fullPath;
FString javacPath;

using namespace std;

UCodeInputWidget::UCodeInputWidget(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
    /*relativePath = FPaths::ProjectContentDir();
    fullPath =  FString("\"") + IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*relativePath) + FString("\"");

    std::string stringPath = "-Djava.class.path=" + std::string(TCHAR_TO_UTF8(*fullPath));

    UE_LOG(LogTemp, Warning, TEXT("Text %s"), *fullPath);

    char * writable = new char[stringPath.size() + 1];
    std::copy(stringPath.begin(), stringPath.end(), writable);
    writable[stringPath.size()] = '\0'; // don't forget the terminating 0

    options[0].optionString = writable;

    std::cout << writable;

    vm_args.version = JNI_VERSION_1_8;
    vm_args.options=options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = 1;

    delete[] writable;

    //Construct JVM
    int res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);*/
}

void UCodeInputWidget::BeginDestroy() {
    /*try {
        vm->DestroyJavaVM();
    }
    catch(...){

    }*/
    Super::BeginDestroy();
}

void UCodeInputWidget::compileCode(FText codeToCompile) {
    relativePath = FPaths::ProjectContentDir();
    fullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*relativePath);
    javacPath = FString("\"") + fullPath + FString("Libraries/jdk1.8.0_211/bin/javac\" ");

    string stringPath = "-Djava.class.path=" + string(TCHAR_TO_UTF8(*fullPath));

    UE_LOG(LogTemp, Warning, TEXT("Text %s"), *fullPath);

    char *writable = new char[stringPath.size() + 1];
    copy(stringPath.begin(), stringPath.end(), writable);
    writable[stringPath.size()] = '\0'; // don't forget the terminating 0

    options[0].optionString = writable;

    vm_args.version = JNI_VERSION_1_8;
    vm_args.options = options;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = 1;

    delete[] writable;

    //Construct JVM
    int res = JNI_CreateJavaVM(&vm, (void **) &env, &vm_args);
    //UE_LOG(LogTemp, Warning, TEXT(UGameplayStatics::GetPlatformName()), res);
    FString codeText = codeToCompile.ToString();
    FString filePath = /*FString("\"") + fullPath + FString("\"") + */FString("UserCode.java");

    cout << TCHAR_TO_ANSI(*filePath);

    bool worked = FFileHelper::SaveStringToFile(codeText, *filePath);

    ofstream userCodeWriter;
    userCodeWriter.open(TCHAR_TO_ANSI(*filePath));
    userCodeWriter << TCHAR_TO_ANSI(*codeText);
    userCodeWriter.close();

    FString javacCommand = (javacPath + filePath);

    string stringCommand = string(TCHAR_TO_UTF8(*javacCommand));

    UE_LOG(LogTemp, Warning, TEXT("File Created: %s"), *javacCommand);
    cout << worked << "\n";

    system(stringCommand.c_str());
    UE_LOG(LogTemp, Warning, TEXT("Class compiled successfuly"));

    try {
        jclass clazz = env->FindClass("UserCode");

        UE_LOG(LogTemp, Warning, TEXT("Class loaded successfuly"));

        // Get the method that you want to call
        jmethodID createMaterial = env->GetMethodID(clazz, "createMaterial", "()Ljava/lang/String;");
        jmethodID constructor = env->GetMethodID(clazz, "<init>", "()V");

        jobject object = env->NewObject(clazz, constructor);
        {
            UE_LOG(LogTemp, Warning, TEXT("Crashed and burned gracefully"));
        }

        UE_LOG(LogTemp, Warning, TEXT("Method loaded successfuly"));

        // Call the method on the object
        jobject result = env->CallObjectMethod(object, createMaterial);

        jstring resultString = (jstring) result;

        // Get a C-style string
        const char *str = env->GetStringUTFChars(resultString, NULL);

        env->ReleaseStringUTFChars(resultString, str);

        UE_LOG(LogTemp, Warning, TEXT("Class method invoked successfuly with result: %s"), str);
        std::cout << str;
    }
    catch (...) {
        UE_LOG(LogTemp, Warning, TEXT("Class couldn't be loaded"));
    }

    //DestroyJavaVM(*vm);
}


