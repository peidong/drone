#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash/uthash.h"
#include "http/http.h"
#include <json-c/json.h>

struct T_pwm {
    const char *pstr_key;          /* key */
    double d_pwm;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct T_pwm* GetPwmStruct()
{
    char *sz_url_get_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/get/";
    /*char *sz_url_post_pwm = "http://fryer.ee.ucla.edu/rest/api/pwm/post/";*/
    
    char* sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_pwm[4], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;
    double pd_pwm[4];
    int n_index=0;

    sz_http_response = http_get(sz_url_get_pwm);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response,"data",&pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm1",&ppT_json_object_pwm[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm2",&ppT_json_object_pwm[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm3",&ppT_json_object_pwm[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm4",&ppT_json_object_pwm[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    n_index = 0;
    for(n_index = 0; n_index < 4; n_index++)
    {
        pd_pwm[n_index] = json_object_get_double(*(ppT_json_object_pwm+n_index));
    }

    const char **kppchIndex, *kstrKeys[] = {"pwm1", "pwm2", "pwm3", "pwm4", NULL};
    struct T_pwm *pT_pwm_selector, *pT_pwm_tmp, *pT_pwm_all = NULL;

    n_index = 0;
    for (kppchIndex = kstrKeys; *kppchIndex != NULL; kppchIndex++) {
        pT_pwm_selector = (struct T_pwm*)malloc(sizeof(struct T_pwm));
        if (pT_pwm_selector == NULL) {
            exit(-1);
        }
        pT_pwm_selector->pstr_key = *kppchIndex;
        pT_pwm_selector->d_pwm = pd_pwm[n_index];
        n_index++;
        HASH_ADD_KEYPTR(hh, pT_pwm_all, pT_pwm_selector->pstr_key, strlen(pT_pwm_selector->pstr_key), pT_pwm_selector);
    }

    return pT_pwm_all;
}

int FreeHashTablePwm(struct T_pwm *pT_pwm_all){
    struct T_pwm *pT_pwm_selector, *pT_pwm_tmp;
    /* free the hash table contents */
    HASH_ITER(hh, pT_pwm_all, pT_pwm_selector, pT_pwm_tmp) {
        HASH_DEL(pT_pwm_all, pT_pwm_selector);
        free(pT_pwm_selector);
    }
    return 0;
} 

double GetPwmValue(struct T_pwm *pT_pwm_all, char *sz_pwm_key)
{
    struct T_pwm *pT_pwm_selector;
    double d_pwm;
    HASH_FIND_STR(pT_pwm_all, sz_pwm_key, pT_pwm_selector);
    if (pT_pwm_selector != NULL) {
        d_pwm = pT_pwm_selector->d_pwm;
    }else{
        d_pwm = 0;
    }
    return d_pwm;
}

/*int main()*/
/*{*/
    /*struct T_pwm *pT_pwm_all = GetPwmStruct();*/
    /*double d_pwm = GetPwmValue(pT_pwm_all, "pwm2");*/
    /*printf("pwm2 = %f", d_pwm);*/
    /*FreeHashTablePwm(pT_pwm_all);*/
    /*return 0;*/
/*}*/
