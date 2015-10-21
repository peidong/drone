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

int get_pwm_struct(char *url_get_pwm, char *url_post_pwm)
{
    //For test
    url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";
    
    struct json_object *jobj;
    char* content;
    struct json_object *pwm1, *data, *pwm2, *pwm3, *pwm4, *update_time;
    int res1, res2;
    content = http_get("http://fryer.ee.ucla.edu/rest/api/pwm/get/");
    jobj = json_tokener_parse(content);
    res1 = json_object_object_get_ex(jobj,"data",&data);
    res2 = json_object_object_get_ex(data,"pwm1",&pwm1);
    res2 = json_object_object_get_ex(data,"pwm2",&pwm2);
    res2 = json_object_object_get_ex(data,"pwm3",&pwm3);
    res2 = json_object_object_get_ex(data,"pwm4",&pwm4);
    res2 = json_object_object_get_ex(data,"update_time",&update_time);
    const char **n, *str_keys[] = {"pwm1", "pwm2", "pwm3", "pwm4"};
    struct pwm_struct *s, *tmp, *users = NULL;
    int i=0;

    for (n = str_keys; *n != NULL; n++) {
        s = (struct pwm_struct*)malloc(sizeof(struct pwm_struct));
        if (s == NULL) {
            exit(-1);
        }
        s->str_key = *n;
        s->pwm_value = i++;
        HASH_ADD_KEYPTR(hh, users, s->str_key, strlen(s->str_key), s );
    }

    HASH_FIND_STR( users, "betty", s);
    if (s != NULL) {
        printf("betty's pwm_value is %f\n", s->pwm_value);
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
