#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

typedef struct JsonValue JsonValue;
typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;

struct JsonValue {
    JsonType type;
    union {
        int bool_val;
        double number_val;
        char *string_val;
        JsonArray *array_val;
        JsonObject *object_val;
    };
};

struct JsonArray {
    JsonValue **values;
    int size;
    int capacity;
};

struct JsonObject {
    char **keys;
    JsonValue **values;
    int size;
    int capacity;
};

typedef struct {
    const char *json;
    int pos;
} Parser;

void skip_whitespace(Parser *p) {
    while (isspace(p->json[p->pos])) {
        p->pos++;
    }
}

JsonValue* parse_value(Parser *p);

JsonValue* create_value(JsonType type) {
    JsonValue *val = malloc(sizeof(JsonValue));
    val->type = type;
    return val;
}

JsonValue* parse_null(Parser *p) {
    if (strncmp(&p->json[p->pos], "null", 4) == 0) {
        p->pos += 4;
        return create_value(JSON_NULL);
    }
    return NULL;
}

JsonValue* parse_bool(Parser *p) {
    if (strncmp(&p->json[p->pos], "true", 4) == 0) {
        p->pos += 4;
        JsonValue *val = create_value(JSON_BOOL);
        val->bool_val = 1;
        return val;
    }
    if (strncmp(&p->json[p->pos], "false", 5) == 0) {
        p->pos += 5;
        JsonValue *val = create_value(JSON_BOOL);
        val->bool_val = 0;
        return val;
    }
    return NULL;
}

JsonValue* parse_number(Parser *p) {
    int start = p->pos;
    if (p->json[p->pos] == '-') p->pos++;
    while (isdigit(p->json[p->pos])) p->pos++;
    if (p->json[p->pos] == '.') {
        p->pos++;
        while (isdigit(p->json[p->pos])) p->pos++;
    }
    if (p->pos > start) {
        char *end;
        double num = strtod(&p->json[start], &end);
        JsonValue *val = create_value(JSON_NUMBER);
        val->number_val = num;
        return val;
    }
    return NULL;
}

JsonValue* parse_string(Parser *p) {
    if (p->json[p->pos] != '"') return NULL;
    p->pos++;
    int start = p->pos;
    while (p->json[p->pos] != '"' && p->json[p->pos] != '\0') {
        if (p->json[p->pos] == '\\') p->pos++;
        p->pos++;
    }
    int len = p->pos - start;
    char *str = malloc(len + 1);
    strncpy(str, &p->json[start], len);
    str[len] = '\0';
    p->pos++;
    JsonValue *val = create_value(JSON_STRING);
    val->string_val = str;
    return val;
}

JsonValue* parse_array(Parser *p) {
    if (p->json[p->pos] != '[') return NULL;
    p->pos++;
    JsonArray *arr = malloc(sizeof(JsonArray));
    arr->capacity = 8;
    arr->size = 0;
    arr->values = malloc(sizeof(JsonValue*) * arr->capacity);

    skip_whitespace(p);
    if (p->json[p->pos] == ']') {
        p->pos++;
        JsonValue *val = create_value(JSON_ARRAY);
        val->array_val = arr;
        return val;
    }

    while (1) {
        skip_whitespace(p);
        JsonValue *item = parse_value(p);
        if (item == NULL) break;

        if (arr->size >= arr->capacity) {
            arr->capacity *= 2;
            arr->values = realloc(arr->values, sizeof(JsonValue*) * arr->capacity);
        }
        arr->values[arr->size++] = item;

        skip_whitespace(p);
        if (p->json[p->pos] == ',') {
            p->pos++;
        } else {
            break;
        }
    }

    skip_whitespace(p);
    if (p->json[p->pos] == ']') {
        p->pos++;
    }

    JsonValue *val = create_value(JSON_ARRAY);
    val->array_val = arr;
    return val;
}

JsonValue* parse_object(Parser *p) {
    if (p->json[p->pos] != '{') return NULL;
    p->pos++;
    JsonObject *obj = malloc(sizeof(JsonObject));
    obj->capacity = 8;
    obj->size = 0;
    obj->keys = malloc(sizeof(char*) * obj->capacity);
    obj->values = malloc(sizeof(JsonValue*) * obj->capacity);

    skip_whitespace(p);
    if (p->json[p->pos] == '}') {
        p->pos++;
        JsonValue *val = create_value(JSON_OBJECT);
        val->object_val = obj;
        return val;
    }

    while (1) {
        skip_whitespace(p);
        JsonValue *key = parse_string(p);
        if (key == NULL) break;

        skip_whitespace(p);
        if (p->json[p->pos] == ':') {
            p->pos++;
        }

        skip_whitespace(p);
        JsonValue *value = parse_value(p);
        if (value == NULL) {
            free(key->string_val);
            free(key);
            break;
        }

        if (obj->size >= obj->capacity) {
            obj->capacity *= 2;
            obj->keys = realloc(obj->keys, sizeof(char*) * obj->capacity);
            obj->values = realloc(obj->values, sizeof(JsonValue*) * obj->capacity);
        }
        obj->keys[obj->size] = key->string_val;
        obj->values[obj->size] = value;
        obj->size++;
        free(key);

        skip_whitespace(p);
        if (p->json[p->pos] == ',') {
            p->pos++;
        } else {
            break;
        }
    }

    skip_whitespace(p);
    if (p->json[p->pos] == '}') {
        p->pos++;
    }

    JsonValue *val = create_value(JSON_OBJECT);
    val->object_val = obj;
    return val;
}

JsonValue* parse_value(Parser *p) {
    skip_whitespace(p);

    JsonValue *val;
    if ((val = parse_null(p)) != NULL) return val;
    if ((val = parse_bool(p)) != NULL) return val;
    if ((val = parse_number(p)) != NULL) return val;
    if ((val = parse_string(p)) != NULL) return val;
    if ((val = parse_array(p)) != NULL) return val;
    if ((val = parse_object(p)) != NULL) return val;

    return NULL;
}

void print_json(JsonValue *val, int indent) {
    if (val == NULL) return;

    switch (val->type) {
        case JSON_NULL:
            printf("null");
            break;
        case JSON_BOOL:
            printf("%s", val->bool_val ? "true" : "false");
            break;
        case JSON_NUMBER:
            printf("%.2f", val->number_val);
            break;
        case JSON_STRING:
            printf("\"%s\"", val->string_val);
            break;
        case JSON_ARRAY:
            printf("[\n");
            for (int i = 0; i < val->array_val->size; i++) {
                for (int j = 0; j < indent + 2; j++) printf(" ");
                print_json(val->array_val->values[i], indent + 2);
                if (i < val->array_val->size - 1) printf(",");
                printf("\n");
            }
            for (int j = 0; j < indent; j++) printf(" ");
            printf("]");
            break;
        case JSON_OBJECT:
            printf("{\n");
            for (int i = 0; i < val->object_val->size; i++) {
                for (int j = 0; j < indent + 2; j++) printf(" ");
                printf("\"%s\": ", val->object_val->keys[i]);
                print_json(val->object_val->values[i], indent + 2);
                if (i < val->object_val->size - 1) printf(",");
                printf("\n");
            }
            for (int j = 0; j < indent; j++) printf(" ");
            printf("}");
            break;
    }
}

int main() {
    const char *json = "{\"name\":\"John\",\"age\":30,\"active\":true,\"balance\":1234.56,\"tags\":[\"developer\",\"admin\"],\"address\":{\"city\":\"NYC\",\"zip\":10001}}";

    printf("Parsing JSON:\n%s\n\n", json);

    Parser parser = {json, 0};
    JsonValue *root = parse_value(&parser);

    if (root != NULL) {
        printf("Parsed successfully:\n");
        print_json(root, 0);
        printf("\n");
    } else {
        printf("Failed to parse JSON\n");
    }

    return 0;
}
