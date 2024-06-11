#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <cassert>

void usage(void)
{
  printf("Usage: virtualkeyboardui-validator JSON [LAYER]\n");
}

void json_p(json_object *jobj)
{
  json_type type = json_object_get_type(jobj);
  switch (type) {
  case json_type_null:
    printf("json_type_null\n");
    break;
  case json_type_boolean:
    printf("json_type_boolean\n");
    break;
  case json_type_double:
    printf("json_type_double\n");
    break;
  case json_type_int:
    printf("json_type_int\n");
    break;
  case json_type_object:
    printf("json_type_object\n");
    break;
  case json_type_array:
    printf("json_type_array\n");
    break;
  case json_type_string:
    printf("json_type_string\n");
    break;
  }
}

typedef struct _Metadata {
  char *label;
  char *languageCode;
  int numberOfLayers;
  char *layerOffset;
  int numberOfStates;
  char *stateLabel;
} Metadata;

int main(int argc, char *argv[])
{
  Metadata meta;
  if (argc < 2) {
    usage();
    exit(1);
  }
  printf("json-c version: %s\n", json_c_version());
  json_object *jobj = json_object_from_file(argv[1]);
  printf("JSON: %s\n", argv[1]);
  json_p(jobj);
  printf("elements: %d\n", json_object_array_length(jobj));  
  for (int i = 0; i < json_object_array_length(jobj); i++) {
    json_object *child = json_object_array_get_idx(jobj, i);
    if (i == 0) {
      assert((void("metadata must be hash"), json_type_object == json_object_get_type(child)));
      // KLE metadata
      json_object_object_foreach(child, key, val) {
        if (!strcmp(key, "Keyboard")) {
          //printf("DEBUG: Keyboard: %s\n", json_object_get_string(keyboard));
          json_object *keyboard = json_object_object_get(child, "Keyboard");
          json_object *value = json_object_object_get(keyboard, "label");
          meta.label = strdup(json_object_get_string(value));
          value = json_object_object_get(keyboard, "languageCode");
          meta.languageCode = strdup(json_object_get_string(value));
          value = json_object_object_get(keyboard, "numberOfLayers");
          meta.numberOfLayers = json_object_get_int64(value);
          value = json_object_object_get(keyboard, "layerOffset");
          meta.layerOffset = strdup(json_object_to_json_string(value));
        } else if (!strcmp(key, "ModeSwitchKey")) {
          //printf("DEBUG key: %s value: %s\n", key, json_object_to_json_string(val));
          json_object *modeswitchkey = json_object_object_get(child, "ModeSwitchKey");
          json_object *value = json_object_object_get(modeswitchkey, "numberOfStates");
          meta.numberOfStates = json_object_get_int64(value);
          value = json_object_object_get(modeswitchkey, "stateLabel");
          meta.stateLabel = strdup(json_object_to_json_string(value));
        } else {
          fprintf(stderr, "unknown key/value in metadata, key: %s value: %s\n", key, json_object_to_json_string(val));
        }
      }
    } else {
      // keyboard layout data
      assert((void("keyboard data must be array"), json_type_array == json_object_get_type(child)));
    }
  }
  printf("Keyboard > label: %s\n", meta.label);
  printf("Keyboard > languageCode: %s\n", meta.languageCode);
  printf("Keyboard > numberOfLayers: %d\n", meta.numberOfLayers);
  printf("Keyboard > layerOffset: %s\n", meta.layerOffset);
  printf("ModeSwitchKey > numberOfStates: %d\n", meta.numberOfStates);
  printf("ModeSwitchKey > stateLabel: %s\n", meta.stateLabel);
}
