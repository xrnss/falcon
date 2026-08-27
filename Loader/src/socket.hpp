#ifndef LOADER_WEBSOCKET_HPP
#define LOADER_WEBSOCKET_HPP

#include <iostream>
#include <string>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include "Java.h"
#include "binary/falconreader.h"
#include "binary/falconwriter.cpp"

using Client = websocketpp::client<websocketpp::config::asio_client>;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

jobject classLoader = nullptr;
int get_client() {
    JVMContext *jvmCtx = JVMContext::instance();
    JNIEnv *env = jvmCtx->jniEnv;
    jvmtiEnv *jvmtiEnvRef = jvmCtx->jvmtiEnv;

    // Attach ctx
    jint result = jvmCtx->vm->GetEnv((void **) (&env), JNI_VERSION_1_8);
    if (result == JNI_EDETACHED) {
        jint attachResult = jvmCtx->vm->AttachCurrentThread((void **) (&env), NULL);
        if (attachResult == JNI_OK) {
            printf("Attachment success!");
        }
    }

    // Find our class loader

    jclass launchClass = env->FindClass("net/minecraft/launchwrapper/launch");
    if (launchClass != NULL) {
        jfieldID fieldID = env->GetStaticFieldID(launchClass, "classLoader", "Lnet/minecraft/launchwrapper/LaunchClassLoader;");
        if (fieldID != NULL) {
            jobject loader = env->GetStaticObjectField(launchClass, fieldID);
            if (loader != NULL) {
                classLoader = env->NewGlobalRef(loader);
                return 1; // Forge
            }

            env->ExceptionClear();
        } else {
            env->ExceptionClear();
        }
    } else {
        env->ExceptionClear();
    }

    jclass mainClass = env->FindClass("net/minecraft/client/main/Main");
    if (mainClass != NULL) {
        jobject mcMainClassLoader;
        jvmtiEnvRef->GetClassLoader(mainClass, &mcMainClassLoader);

        classLoader = env->NewGlobalRef(mcMainClassLoader);
        return 2; // Vanilla
    } else {
        env->ExceptionClear();
    }

    if (launchClass != NULL) {
        jint classCount = 0;
        jclass* classes = nullptr;
        jvmtiEnvRef->GetLoadedClasses(&classCount, &classes);

        for (int i = 0; i < classCount; i++) {
            char* signature = nullptr;
            if (jvmtiEnvRef->GetClassSignature(classes[i], &signature, nullptr) == JVMTI_ERROR_NONE) {
                if (signature && strncmp(signature, "Lnet/minecraft/", 15) == 0) {
                    jclass cls = classes[i];
                    jclass classClass = env->FindClass("java/lang/Class");
                    jmethodID getClassLoader = env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
                    jobject loader = env->CallObjectMethod(cls, getClassLoader);

                    if (loader != nullptr) {
                        classLoader = env->NewGlobalRef(loader);
                        return 3; // Lunar
                    }
                }
            }
        }
    }

    return 0; // Failed
}

std::vector<uint8_t> base64Decode(const std::string &base64String) {
    // Base64 decoding table
    const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::vector<uint8_t> result;
    int val = 0;
    int valb = -8;
    for (char c: base64String) {
        if (std::isspace(c)) {
            continue; // Skip whitespace
        } else if (c == '=') {
            break; // Padding found, stop decoding
        } else {
            size_t idx = base64Chars.find(c);
            if (idx == std::string::npos) {
                std::cerr << "Invalid character in base64 string: " << c << std::endl;
                return std::vector<uint8_t>();
            }
            val = (val << 6) + static_cast<int>(idx);
            valb += 6;

            if (valb >= 0) {
                result.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
    }

    return result;
}

jclass FindClassWithLoader(JNIEnv* env, jobject loader, const std::string& classNameSlash) {
    if (loader == nullptr) {
        // fallback to system FindClass if loader not set
        return env->FindClass(classNameSlash.c_str());
    }

    // convert slash notation to dot notation
    std::string classNameDot = classNameSlash;
    std::replace(classNameDot.begin(), classNameDot.end(), '/', '.');

    // get loader class
    jclass classLoaderClass = env->GetObjectClass(loader);
    if (classLoaderClass == nullptr) {
        return nullptr;
    }

    // get loadClass method id
    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    if (loadClassMethod == nullptr) {
        env->DeleteLocalRef(classLoaderClass);
        return nullptr;
    }

    // create jstring for class name
    jstring jClassName = env->NewStringUTF(classNameDot.c_str());
    if (jClassName == nullptr) {
        env->DeleteLocalRef(classLoaderClass);
        return nullptr;
    }

    // call loadClass
    jclass clazz = (jclass) env->CallObjectMethod(loader, loadClassMethod, jClassName);

    // clean up
    env->DeleteLocalRef(jClassName);
    env->DeleteLocalRef(classLoaderClass);

    // check for exceptions
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        return nullptr;
    }

    return clazz;
}

void on_message(Client *c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::vector<uint8_t> payloadData(msg->get_payload().begin(), msg->get_payload().end());
    FalconReader reader(payload, 0, false);
    JVMContext *jvmCtx = JVMContext::instance();

    int opCode = reader.getUint8();
    switch (opCode) {
        case 1: {
            // Handshake [A]
            // Any form of custom logic desired can go here.
            // For our example target, Minecraft, we shall determine which client type we are loaded into and communicate this.

            int clientType = get_client();
            FalconWriter writer;
            writer.setPacketId(1); // Handshake [B]
            writer.setUint8(clientType);

            std::vector<uint8_t> data = writer.build();
            std::string payloadStr(data.begin(), data.end());

            websocketpp::lib::error_code ec2;
            c->send(hdl, payloadStr, websocketpp::frame::opcode::BINARY, ec2);

            if (ec2) {
                std::cout << "could not send payload: " << ec2.message() << std::endl;
            }

            break;
        }
        case 2: {
            // Class [A]
            JNIEnv *env = jvmCtx->jniEnv;

            jint result = jvmCtx->vm->GetEnv((void **) (&env), JNI_VERSION_1_8);
            jint attachResult = jvmCtx->vm->AttachCurrentThread((void **) (&env), NULL);
            if (attachResult == JNI_OK) {
                printf("attached\n");
            } else {
                printf("failed to attach\n");
            }

            struct PendingClass {
                std::string name;
                std::vector<uint8_t> bytes;
            };
            std::vector<PendingClass> pendingClasses;

            int classCount = reader.getUint32();
            for (int index = 0; index < classCount; index++) {
                std::string className = reader.getStringUTF8();
                std::string classBytesB64 = reader.getStringUTF8();

                // Skip module descriptors
                if (className == "module-info" || className.find("module-info") != std::string::npos) {
                    continue;
                }

                PendingClass pc;
                pc.name = className;
                pc.bytes = base64Decode(classBytesB64);

                pendingClasses.push_back(std::move(pc));
            }

            // We use a resilient class loading system, where classes that fail to load (usually due to dependency issues) are retried later
            while (!pendingClasses.empty()) {
                bool progress = false;

                for (auto it = pendingClasses.begin(); it != pendingClasses.end();) {
                    jbyteArray jByteArray = env->NewByteArray(it->bytes.size());
                    env->SetByteArrayRegion(jByteArray, 0, it->bytes.size(), reinterpret_cast<jbyte*>(it->bytes.data()));
                    jbyte* byteArrayElements = env->GetByteArrayElements(jByteArray, NULL);

                    jclass definedClass = env->DefineClass(
                                it->name.c_str(),
                                classLoader,
                                byteArrayElements,
                                it->bytes.size()
                        );

                    env->ReleaseByteArrayElements(
                            jByteArray,
                            byteArrayElements,
                            JNI_ABORT
                    );

                    env->DeleteLocalRef(jByteArray);

                    if (env->ExceptionCheck() || definedClass == nullptr) {
                        // Dependency probably missing, try again later
                        env->ExceptionClear();
                        ++it;
                    }
                    else {
                        printf("Defined class: %s\n", it->name.c_str());

                        it = pendingClasses.erase(it);
                        progress = true;
                    }
                }

                if (!progress) {
                    std::cout << "Could not resolve remaining classes:" << std::endl;

                    for (auto& cls : pendingClasses) {
                        std::cout << "  " << cls.name << std::endl;
                    }

                    break;
                }
            }

            // Load bootstrap class
            std::string bootstrapName = reader.getStringUTF8();
            jclass bootstrapClass = FindClassWithLoader(env, classLoader, bootstrapName);

            if (bootstrapClass == NULL) {
                std::cout << "could not find class: " << bootstrapName << std::endl;
            }

            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
            }

            std::string methodName = reader.getStringUTF8();
            std::string methodSignature = reader.getStringUTF8();

            jmethodID bootstrapMethodID = env->GetStaticMethodID(bootstrapClass, methodName.c_str(), methodSignature.c_str());
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
            }

            if (bootstrapMethodID == NULL) {
                std::cout << "could not find method: " << methodName << std::endl;
            }

            env->CallStaticVoidMethod(bootstrapClass, bootstrapMethodID);

            std::cout << "Hooking method called!" << std::endl;

            FalconWriter writer;
            writer.setPacketId(2); // Client [B]
            std::vector<uint8_t> data = writer.build();
            std::string payloadStr(data.begin(), data.end());

            websocketpp::lib::error_code ec2;
            c->send(hdl, payloadStr, websocketpp::frame::opcode::BINARY, ec2);
        }
    }
}

void connect(int port) {
    Client client;

    client.init_asio();
    client.set_access_channels(websocketpp::log::alevel::all);
    client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    client.clear_access_channels(websocketpp::log::alevel::frame_header);

    client.set_message_handler(std::bind(&on_message, &client, std::placeholders::_1, std::placeholders::_2));

    websocketpp::lib::error_code ec;
    Client::connection_ptr con = client.get_connection(std::string("ws://localhost:") + std::to_string(port), ec);
    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
        return;
    }
    client.connect(con);

    client.run();
}

#endif