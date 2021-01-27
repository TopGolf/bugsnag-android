#include "metadata.h"
#include "safejni.h"
#include "utils/string.h"
#include <malloc.h>
#include <string.h>

typedef struct {
  jclass hash_map;
  jclass map;
  jclass arraylist;
  jclass integer;
  jclass boolean;
  jclass metadata;
  jclass native_interface;
  jclass long_class;
  jclass float_class;
  jclass number;
  jclass string;
  jmethodID integer_int_value;
  jmethodID long_long_value;
  jmethodID float_float_value;
  jmethodID boolean_bool_value;
  jmethodID number_double_value;
  jmethodID hash_map_get;
  jmethodID hash_map_size;
  jmethodID hash_map_key_set;
  jmethodID map_get;
  jmethodID map_size;
  jmethodID map_key_set;
  jmethodID arraylist_init_with_obj;
  jmethodID arraylist_get;
  jmethodID get_app_data;
  jmethodID get_device_data;
  jmethodID get_user_data;
  jmethodID get_breadcrumbs;
  jmethodID get_metadata;
  jmethodID get_context;
} bsg_jni_cache;

void bsg_populate_metadata_value(JNIEnv *env, bugsnag_metadata *dst,
                                 bsg_jni_cache *jni_cache, char *section,
                                 char *name, jobject _value);

/**
 * Creates a cache of JNI methods/classes that are commonly used.
 *
 * Class and method objects can be kept safely since they aren't moved or
 * removed from the JVM - care should be taken not to load objects as local
 * references here.
 */
bsg_jni_cache *bsg_populate_jni_cache(JNIEnv *env) {
  bsg_jni_cache *jni_cache = malloc(sizeof(bsg_jni_cache));

  // lookup java/lang/Integer
  jni_cache->integer = bsg_safe_find_class(env, "java/lang/Integer");
  if (jni_cache->integer == NULL) {
    return NULL;
  }

  // lookup java/lang/Boolean
  jni_cache->boolean = bsg_safe_find_class(env, "java/lang/Boolean");
  if (jni_cache->boolean == NULL) {
    return NULL;
  }

  // lookup java/lang/Long
  jni_cache->long_class = bsg_safe_find_class(env, "java/lang/Long");
  if (jni_cache->long_class == NULL) {
    return NULL;
  }

  // lookup java/lang/Float
  jni_cache->float_class = bsg_safe_find_class(env, "java/lang/Float");
  if (jni_cache->float_class == NULL) {
    return NULL;
  }

  // lookup java/lang/Number
  jni_cache->number = bsg_safe_find_class(env, "java/lang/Number");
  if (jni_cache->number == NULL) {
    return NULL;
  }

  // lookup java/lang/String
  jni_cache->string = bsg_safe_find_class(env, "java/lang/String");
  if (jni_cache->string == NULL) {
    return NULL;
  }

  // lookup Integer.intValue()
  jni_cache->integer_int_value =
      bsg_safe_get_method_id(env, jni_cache->integer, "intValue", "()I");
  if (jni_cache->integer_int_value == NULL) {
    return NULL;
  }

  // lookup Integer.floatValue()
  jni_cache->float_float_value =
      bsg_safe_get_method_id(env, jni_cache->float_class, "floatValue", "()F");
  if (jni_cache->float_float_value == NULL) {
    return NULL;
  }

  // lookup Double.doubleValue()
  jni_cache->number_double_value =
      bsg_safe_get_method_id(env, jni_cache->number, "doubleValue", "()D");
  if (jni_cache->number_double_value == NULL) {
    return NULL;
  }

  // lookup Long.longValue()
  jni_cache->long_long_value =
      bsg_safe_get_method_id(env, jni_cache->integer, "longValue", "()J");
  if (jni_cache->long_long_value == NULL) {
    return NULL;
  }

  // lookup Boolean.booleanValue()
  jni_cache->boolean_bool_value =
      bsg_safe_get_method_id(env, jni_cache->boolean, "booleanValue", "()Z");
  if (jni_cache->boolean_bool_value == NULL) {
    return NULL;
  }

  // lookup java/util/ArrayList
  jni_cache->arraylist = bsg_safe_find_class(env, "java/util/ArrayList");
  if (jni_cache->arraylist == NULL) {
    return NULL;
  }

  // lookup ArrayList constructor
  jni_cache->arraylist_init_with_obj = bsg_safe_get_method_id(
      env, jni_cache->arraylist, "<init>", "(Ljava/util/Collection;)V");
  if (jni_cache->arraylist_init_with_obj == NULL) {
    return NULL;
  }

  // lookup ArrayList.get()
  jni_cache->arraylist_get = bsg_safe_get_method_id(
      env, jni_cache->arraylist, "get", "(I)Ljava/lang/Object;");
  if (jni_cache->arraylist_get == NULL) {
    return NULL;
  }

  // lookup java/util/HashMap
  jni_cache->hash_map = bsg_safe_find_class(env, "java/util/HashMap");
  if (jni_cache->hash_map == NULL) {
    return NULL;
  }

  // lookup java/util/Map
  jni_cache->map = bsg_safe_find_class(env, "java/util/Map");
  if (jni_cache->map == NULL) {
    return NULL;
  }

  // lookup java/util/Set
  jni_cache->hash_map_key_set = bsg_safe_get_method_id(
      env, jni_cache->hash_map, "keySet", "()Ljava/util/Set;");
  if (jni_cache->hash_map_key_set == NULL) {
    return NULL;
  }

  // lookup HashMap.size()
  jni_cache->hash_map_size =
      bsg_safe_get_method_id(env, jni_cache->hash_map, "size", "()I");
  if (jni_cache->hash_map_size == NULL) {
    return NULL;
  }

  // lookup HashMap.get()
  jni_cache->hash_map_get =
      bsg_safe_get_method_id(env, jni_cache->hash_map, "get",
                             "(Ljava/lang/Object;)Ljava/lang/Object;");
  if (jni_cache->hash_map_get == NULL) {
    return NULL;
  }

  // lookup Map.keySet()
  jni_cache->map_key_set = bsg_safe_get_method_id(env, jni_cache->map, "keySet",
                                                  "()Ljava/util/Set;");
  if (jni_cache->map_key_set == NULL) {
    return NULL;
  }

  // lookup Map.size()
  jni_cache->map_size =
      bsg_safe_get_method_id(env, jni_cache->map, "size", "()I");
  if (jni_cache->map_size == NULL) {
    return NULL;
  }

  // lookup Map.get()
  jni_cache->map_get = bsg_safe_get_method_id(
      env, jni_cache->map, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
  if (jni_cache->map_get == NULL) {
    return NULL;
  }

  // lookup com/bugsnag/android/NativeInterface
  jni_cache->native_interface =
      bsg_safe_find_class(env, "com/bugsnag/android/NativeInterface");
  if (jni_cache->native_interface == NULL) {
    return NULL;
  }

  // lookup NativeInterface.getApp()
  jni_cache->get_app_data = bsg_safe_get_static_method_id(
      env, jni_cache->native_interface, "getApp", "()Ljava/util/Map;");
  if (jni_cache->get_app_data == NULL) {
    return NULL;
  }

  // lookup NativeInterface.getDevice()
  jni_cache->get_device_data = bsg_safe_get_static_method_id(
      env, jni_cache->native_interface, "getDevice", "()Ljava/util/Map;");
  if (jni_cache->get_device_data == NULL) {
    return NULL;
  }

  // lookup NativeInterface.getUser()
  jni_cache->get_user_data = bsg_safe_get_static_method_id(
      env, jni_cache->native_interface, "getUser", "()Ljava/util/Map;");
  if (jni_cache->get_user_data == NULL) {
    return NULL;
  }

  // lookup NativeInterface.getMetadata()
  jni_cache->get_metadata = bsg_safe_get_static_method_id(
      env, jni_cache->native_interface, "getMetadata", "()Ljava/util/Map;");
  if (jni_cache->get_metadata == NULL) {
    return NULL;
  }

  // lookup NativeInterface.getContext()
  jni_cache->get_context = bsg_safe_get_static_method_id(
      env, jni_cache->native_interface, "getContext", "()Ljava/lang/String;");
  if (jni_cache->get_context == NULL) {
    return NULL;
  }
  return jni_cache;
}

jobject bsg_get_map_value_obj(JNIEnv *env, bsg_jni_cache *jni_cache,
                              jobject map, const char *_key) {
  jstring key = (*env)->NewStringUTF(env, _key);
  jobject obj =
      (*env)->CallObjectMethod(env, map, jni_cache->hash_map_get, key);
  (*env)->DeleteLocalRef(env, key);
  return obj;
}

void bsg_copy_map_value_string(JNIEnv *env, bsg_jni_cache *jni_cache,
                               jobject map, const char *_key, char *dest,
                               int len) {
  jobject _value = bsg_get_map_value_obj(env, jni_cache, map, _key);

  if (_value != NULL) {
    char *value = (char *)(*env)->GetStringUTFChars(env, (jstring)_value, 0);
    bsg_strncpy_safe(dest, value, len);
    (*env)->ReleaseStringUTFChars(env, _value, value);
  }
}

long bsg_get_map_value_long(JNIEnv *env, bsg_jni_cache *jni_cache, jobject map,
                            const char *_key) {
  jobject _value = bsg_get_map_value_obj(env, jni_cache, map, _key);

  if (_value != NULL) {
    long value = (long)(*env)->CallDoubleMethod(env, _value,
                                                jni_cache->number_double_value);
    (*env)->DeleteLocalRef(env, _value);
    return value;
  }
  return 0;
}

float bsg_get_map_value_float(JNIEnv *env, bsg_jni_cache *jni_cache,
                              jobject map, const char *_key) {
  jobject _value = bsg_get_map_value_obj(env, jni_cache, map, _key);

  if (_value != NULL) {
    float value = (float)(*env)->CallFloatMethod(env, _value,
                                                 jni_cache->float_float_value);
    (*env)->DeleteLocalRef(env, _value);
    return value;
  }
  return 0;
}

int bsg_get_map_value_int(JNIEnv *env, bsg_jni_cache *jni_cache, jobject map,
                          const char *_key) {
  jobject _value = bsg_get_map_value_obj(env, jni_cache, map, _key);

  if (_value != NULL) {
    jint value =
        (int)(*env)->CallIntMethod(env, _value, jni_cache->integer_int_value);
    (*env)->DeleteLocalRef(env, _value);
    return value;
  }
  return 0;
}

bool bsg_get_map_value_bool(JNIEnv *env, bsg_jni_cache *jni_cache, jobject map,
                            const char *_key) {
  jobject obj = bsg_get_map_value_obj(env, jni_cache, map, _key);
  return (*env)->CallBooleanMethod(env, obj, jni_cache->boolean_bool_value);
}

int bsg_populate_cpu_abi_from_map(JNIEnv *env, bsg_jni_cache *jni_cache,
                                  jobject map, bsg_device_info *device) {
  jstring key = (*env)->NewStringUTF(env, "cpuAbi");
  jobjectArray _value =
      (*env)->CallObjectMethod(env, map, jni_cache->hash_map_get, key);
  if (_value != NULL) {
    int count = (*env)->GetArrayLength(env, _value);

    for (int i = 0; i < count && i < sizeof(device->cpu_abi); i++) {
      jstring abi_ = (jstring)((*env)->GetObjectArrayElement(env, _value, i));
      char *abi = (char *)(*env)->GetStringUTFChars(env, abi_, 0);
      bsg_strncpy_safe(device->cpu_abi[i].value, abi,
                       sizeof(device->cpu_abi[i].value));
      (*env)->ReleaseStringUTFChars(env, abi_, abi);
      device->cpu_abi_count++;
    }
    (*env)->DeleteLocalRef(env, _value);
    return count;
  }
  return 0;
}

void bsg_populate_crumb_metadata(JNIEnv *env, bugsnag_breadcrumb *crumb,
                                 jobject metadata) {
  if (metadata == NULL) {
    return;
  }
  bsg_jni_cache *jni_cache = bsg_populate_jni_cache(env);
  if (jni_cache == NULL) {
    return;
  }

  int map_size = (int)(*env)->CallIntMethod(env, metadata, jni_cache->map_size);
  jobject keyset =
      (*env)->CallObjectMethod(env, metadata, jni_cache->map_key_set);
  jobject keylist = (*env)->NewObject(
      env, jni_cache->arraylist, jni_cache->arraylist_init_with_obj, keyset);

  for (int i = 0; i < map_size; i++) {
    jstring _key = (*env)->CallObjectMethod(env, keylist,
                                            jni_cache->arraylist_get, (jint)i);
    jobject _value =
        (*env)->CallObjectMethod(env, metadata, jni_cache->map_get, _key);

    if (_key == NULL || _value == NULL) {
      (*env)->DeleteLocalRef(env, _key);
      (*env)->DeleteLocalRef(env, _value);
    } else {
      char *key = (char *)(*env)->GetStringUTFChars(env, _key, 0);
      bsg_populate_metadata_value(env, &crumb->metadata, jni_cache, "metaData",
                                  key, _value);
      (*env)->ReleaseStringUTFChars(env, _key, key);
    }
  }
  free(jni_cache);
  (*env)->DeleteLocalRef(env, keyset);
  (*env)->DeleteLocalRef(env, keylist);
}

char *bsg_binary_arch() {
#if defined(__i386__)
  return "x86";
#elif defined(__x86_64__)
  return "x86_64";
#elif defined(__arm__)
  return "arm32";
#elif defined(__aarch64__)
  return "arm64";
#else
  return "unknown";
#endif
}

void bsg_populate_app_data(JNIEnv *env, bsg_jni_cache *jni_cache,
                           bugsnag_event *event) {
  jobject data = (*env)->CallStaticObjectMethod(
      env, jni_cache->native_interface, jni_cache->get_app_data);

  bsg_strncpy_safe(event->app.binary_arch, bsg_binary_arch(),
                   sizeof(event->app.binary_arch));

  bsg_copy_map_value_string(env, jni_cache, data, "buildUUID",
                            event->app.build_uuid,
                            sizeof(event->app.build_uuid));
  event->app.duration_ms_offset =
      bsg_get_map_value_long(env, jni_cache, data, "duration");
  event->app.duration_in_foreground_ms_offset =
      bsg_get_map_value_long(env, jni_cache, data, "durationInForeground");

  bsg_copy_map_value_string(env, jni_cache, data, "id", event->app.id,
                            sizeof(event->app.id));
  event->app.in_foreground =
      bsg_get_map_value_bool(env, jni_cache, data, "inForeground");

  char name[64];
  bsg_copy_map_value_string(env, jni_cache, data, "name", name, sizeof(name));
  bugsnag_event_add_metadata_string(event, "app", "name", name);

  bsg_copy_map_value_string(env, jni_cache, data, "releaseStage",
                            event->app.release_stage,
                            sizeof(event->app.release_stage));
  bsg_copy_map_value_string(env, jni_cache, data, "type", event->app.type,
                            sizeof(event->app.type));
  bsg_copy_map_value_string(env, jni_cache, data, "version", event->app.version,
                            sizeof(event->app.version));
  event->app.version_code =
      bsg_get_map_value_int(env, jni_cache, data, "versionCode");

  (*env)->DeleteLocalRef(env, data);
}

char *bsg_os_name() { return "android"; }

void populate_device_metadata(JNIEnv *env, bsg_jni_cache *jni_cache,
                              bugsnag_event *event, void *data) {
  char brand[64];
  bsg_copy_map_value_string(env, jni_cache, data, "brand", brand,
                            sizeof(brand));
  bugsnag_event_add_metadata_string(event, "device", "brand", brand);

  bugsnag_event_add_metadata_double(
      event, "device", "dpi",
      bsg_get_map_value_int(env, jni_cache, data, "dpi"));
  bugsnag_event_add_metadata_bool(
      event, "device", "emulator",
      bsg_get_map_value_bool(env, jni_cache, data, "emulator"));

  char location_status[32];
  bsg_copy_map_value_string(env, jni_cache, data, "locationStatus",
                            location_status, sizeof(location_status));
  bugsnag_event_add_metadata_string(event, "device", "locationStatus",
                                    location_status);

  char network_access[64];
  bsg_copy_map_value_string(env, jni_cache, data, "networkAccess",
                            network_access, sizeof(network_access));
  bugsnag_event_add_metadata_string(event, "device", "networkAccess",
                                    network_access);

  bugsnag_event_add_metadata_double(
      event, "device", "screenDensity",
      bsg_get_map_value_float(env, jni_cache, data, "screenDensity"));

  char screen_resolution[32];
  bsg_copy_map_value_string(env, jni_cache, data, "screenResolution",
                            screen_resolution, sizeof(screen_resolution));
  bugsnag_event_add_metadata_string(event, "device", "screenResolution",
                                    screen_resolution);
}

void bsg_populate_device_data(JNIEnv *env, bsg_jni_cache *jni_cache,
                              bugsnag_event *event) {
  jobject data = (*env)->CallStaticObjectMethod(
      env, jni_cache->native_interface, jni_cache->get_device_data);

  bsg_populate_cpu_abi_from_map(env, jni_cache, data, &event->device);

  bsg_copy_map_value_string(env, jni_cache, data, "id", event->device.id,
                            sizeof(event->device.id));
  event->device.jailbroken =
      bsg_get_map_value_bool(env, jni_cache, data, "jailbroken");

  bsg_copy_map_value_string(env, jni_cache, data, "locale",
                            event->device.locale, sizeof(event->device.locale));

  bsg_copy_map_value_string(env, jni_cache, data, "manufacturer",
                            event->device.manufacturer,
                            sizeof(event->device.manufacturer));
  bsg_copy_map_value_string(env, jni_cache, data, "model", event->device.model,
                            sizeof(event->device.model));

  bsg_copy_map_value_string(env, jni_cache, data, "orientation",
                            event->device.orientation,
                            sizeof(event->device.orientation));
  bsg_strcpy(event->device.os_name, bsg_os_name());
  bsg_copy_map_value_string(env, jni_cache, data, "osVersion",
                            event->device.os_version,
                            sizeof(event->device.os_version));

  jobject _runtime_versions =
      bsg_get_map_value_obj(env, jni_cache, data, "runtimeVersions");

  if (_runtime_versions != NULL) {
    bsg_copy_map_value_string(env, jni_cache, _runtime_versions, "osBuild",
                              event->device.os_build,
                              sizeof(event->device.os_build));

    event->device.api_level = bsg_get_map_value_int(
        env, jni_cache, _runtime_versions, "androidApiLevel");
    (*env)->DeleteLocalRef(env, _runtime_versions);
  }
  event->device.total_memory =
      bsg_get_map_value_long(env, jni_cache, data, "totalMemory");

  // add fields to device metadata
  populate_device_metadata(env, jni_cache, event, data);
  (*env)->DeleteLocalRef(env, data);
}

void bsg_populate_user_data(JNIEnv *env, bsg_jni_cache *jni_cache,
                            bugsnag_event *event) {
  jobject data = (*env)->CallStaticObjectMethod(
      env, jni_cache->native_interface, jni_cache->get_user_data);
  bsg_copy_map_value_string(env, jni_cache, data, "id", event->user.id,
                            sizeof(event->user.id));
  bsg_copy_map_value_string(env, jni_cache, data, "name", event->user.name,
                            sizeof(event->user.name));
  bsg_copy_map_value_string(env, jni_cache, data, "email", event->user.email,
                            sizeof(event->user.email));
  (*env)->DeleteLocalRef(env, data);
}

void bsg_populate_context(JNIEnv *env, bsg_jni_cache *jni_cache,
                          bugsnag_event *event) {
  jstring _context = (*env)->CallStaticObjectMethod(
      env, jni_cache->native_interface, jni_cache->get_context);
  if (_context != NULL) {
    const char *value = (*env)->GetStringUTFChars(env, (jstring)_context, 0);
    strncpy(event->context, value, sizeof(event->context) - 1);
    (*env)->ReleaseStringUTFChars(env, _context, value);
  } else {
    memset(&event->context, 0, strlen(event->context));
  }
}

void bsg_populate_event(JNIEnv *env, bugsnag_event *event) {
  bsg_jni_cache *jni_cache = bsg_populate_jni_cache(env);
  if (jni_cache == NULL) {
    return;
  }
  bsg_populate_context(env, jni_cache, event);
  bsg_populate_app_data(env, jni_cache, event);
  bsg_populate_device_data(env, jni_cache, event);
  bsg_populate_user_data(env, jni_cache, event);
  free(jni_cache);
}

void bsg_populate_metadata_value(JNIEnv *env, bugsnag_metadata *dst,
                                 bsg_jni_cache *jni_cache, char *section,
                                 char *name, jobject _value) {
  if ((*env)->IsInstanceOf(env, _value, jni_cache->number)) {
    double value =
        (*env)->CallDoubleMethod(env, _value, jni_cache->number_double_value);
    bsg_add_metadata_value_double(dst, section, name, value);
  } else if ((*env)->IsInstanceOf(env, _value, jni_cache->boolean)) {
    bool value =
        (*env)->CallBooleanMethod(env, _value, jni_cache->boolean_bool_value);
    bsg_add_metadata_value_bool(dst, section, name, value);
  } else if ((*env)->IsInstanceOf(env, _value, jni_cache->string)) {
    char *value = (char *)(*env)->GetStringUTFChars(env, _value, 0);
    bsg_add_metadata_value_str(dst, section, name, value);
    free(value);
  }
}

void bsg_populate_metadata(JNIEnv *env, bugsnag_metadata *dst,
                           jobject metadata) {
  bsg_jni_cache *jni_cache = bsg_populate_jni_cache(env);
  if (jni_cache == NULL) {
    return;
  }
  if (metadata == NULL) {
    metadata = (*env)->CallStaticObjectMethod(env, jni_cache->native_interface,
                                              jni_cache->get_metadata);
  }
  if (metadata != NULL) {
    int size = (int)(*env)->CallIntMethod(env, metadata, jni_cache->map_size);
    jobject keyset =
        (*env)->CallObjectMethod(env, metadata, jni_cache->map_key_set);
    jobject keylist = (*env)->NewObject(
        env, jni_cache->arraylist, jni_cache->arraylist_init_with_obj, keyset);
    for (int i = 0; i < size; i++) {
      jstring _key = (*env)->CallObjectMethod(
          env, keylist, jni_cache->arraylist_get, (jint)i);
      char *section = (char *)(*env)->GetStringUTFChars(env, _key, 0);
      jobject _section =
          (*env)->CallObjectMethod(env, metadata, jni_cache->map_get, _key);
      int section_size =
          (int)(*env)->CallIntMethod(env, _section, jni_cache->map_size);
      jobject section_keyset =
          (*env)->CallObjectMethod(env, _section, jni_cache->map_key_set);
      jobject section_keylist =
          (*env)->NewObject(env, jni_cache->arraylist,
                            jni_cache->arraylist_init_with_obj, section_keyset);
      for (int j = 0; j < section_size; j++) {
        jstring section_key = (*env)->CallObjectMethod(
            env, section_keylist, jni_cache->arraylist_get, (jint)j);
        char *name = (char *)(*env)->GetStringUTFChars(env, section_key, 0);
        jobject _value = (*env)->CallObjectMethod(
            env, section, jni_cache->map_get, section_key);
        bsg_populate_metadata_value(env, dst, jni_cache, section, name, _value);
        (*env)->ReleaseStringUTFChars(env, section_key, name);
        (*env)->DeleteLocalRef(env, _value);
      }
      (*env)->ReleaseStringUTFChars(env, _key, section);
      (*env)->DeleteLocalRef(env, section_keyset);
      (*env)->DeleteLocalRef(env, section_keylist);
      (*env)->DeleteLocalRef(env, _section);
    }
    (*env)->DeleteLocalRef(env, keyset);
    (*env)->DeleteLocalRef(env, keylist);
  } else {
    dst->value_count = 0;
  }
  free(jni_cache);
}
