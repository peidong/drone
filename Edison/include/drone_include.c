#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash/uthash.h"
#include "http/http.h"
#include <json-c/json.h>

struct pwm_struct {
    const char *str_key;          /* key */
    double pwm_value;
    UT_hash_handle hh;         /* makes this structure hashable */
};

int get_pwm_struct()
{
    //For test
    char *url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    char *url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";
    
    char* str_http_response;
    struct json_object *json_object_whole_response, *json_object_data, *json_object_pwm1, *json_object_pwm2, *json_object_pwm3, *json_object_pwm4, *json_object_update_time;
    int tag_res;

    str_http_response = http_get(url_get_pwm);

    json_object_whole_response = json_tokener_parse(str_http_response);

    tag_res = json_object_object_get_ex(json_object_whole_response,"data",&json_object_data);
    tag_res = json_object_object_get_ex(json_object_data,"pwm1",&json_object_pwm1);
    tag_res = json_object_object_get_ex(json_object_data,"pwm2",&json_object_pwm2);
    tag_res = json_object_object_get_ex(json_object_data,"pwm3",&json_object_pwm3);
    tag_res = json_object_object_get_ex(json_object_data,"pwm4",&json_object_pwm4);
    tag_res = json_object_object_get_ex(json_object_data,"update_time",&json_object_update_time);

    const char **index, *str_keys[] = {"pwm1", "pwm2", "pwm3", "pwm4"};
    struct pwm_struct *s, *tmp, *users = NULL;
    int i=0;

    for (index = str_keys; *index != NULL; index++) {
        s = (struct pwm_struct*)malloc(sizeof(struct pwm_struct));
        if (s == NULL) {
            exit(-1);
        }
        s->str_key = *index;
        s->pwm_value = i++;
        HASH_ADD_KEYPTR(hh, users, s->str_key, strlen(s->str_key), s );
    }

    HASH_FIND_STR( users, "betty", s);
    if (s != NULL) {
        printf("betty's pwm_value is %f\index", s->pwm_value);
    }

    /* free the hash table contents */
    HASH_ITER(hh, users, s, tmp) {
        HASH_DEL(users, s);
        free(s);
    }
    return 0;
}

int main()
{
    return 0;
}
