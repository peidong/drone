#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash/uthash.h"
#include "http/http.h"
#include <json-c/json.h>
#include "thread/thpool.h" /*https://github.com/Pithikos/C-Thread-Pool*/
#include <unistd.h>/*usleep*/


struct T_pwm {
    const char *pstr_key;          /* key */
    double d_pwm;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct T_pwm* HTTP_get_pT_pwm()
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
    struct T_pwm *pT_pwm_selector, *pT_pwm_all = NULL;

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

int free_pT_pwm(struct T_pwm *pT_pwm_all){
    struct T_pwm *pT_pwm_selector, *pT_pwm_tmp;
    /* free the hash table contents */
    HASH_ITER(hh, pT_pwm_all, pT_pwm_selector, pT_pwm_tmp) {
        HASH_DEL(pT_pwm_all, pT_pwm_selector);
        free(pT_pwm_selector);
    }
    return 0;
} 

double get_d_pwm(struct T_pwm *pT_pwm_all, char *sz_pwm_key)
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

struct T_control {
    const char *pstr_key;          /* key */
    double d_control;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct T_control* HTTP_get_pT_control()
{
    char *sz_url_get_control = "http://fryer.ee.ucla.edu/rest/api/control/get/";
    /*char *sz_url_post_control = "http://fryer.ee.ucla.edu/rest/api/control/post/";*/
    
    char* sz_http_response;
    struct json_object *pT_json_object_whole_response, *ppT_json_object_control[4], *pT_json_object_data, *pT_json_object_update_time;
    int n_json_response;
    double pd_control[4];
    int n_index=0;

    sz_http_response = http_get(sz_url_get_control);

    pT_json_object_whole_response = json_tokener_parse(sz_http_response);

    n_json_response = json_object_object_get_ex(pT_json_object_whole_response,"data",&pT_json_object_data);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm1",&ppT_json_object_control[0]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm2",&ppT_json_object_control[1]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm3",&ppT_json_object_control[2]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"pwm4",&ppT_json_object_control[3]);
    n_json_response = json_object_object_get_ex(pT_json_object_data,"update_time",&pT_json_object_update_time);

    n_index = 0;
    for(n_index = 0; n_index < 4; n_index++)
    {
        pd_control[n_index] = json_object_get_double(*(ppT_json_object_control+n_index));
    }

    const char **kppchIndex, *kstrKeys[] = {"pwm1", "pwm2", "pwm3", "pwm4", NULL};
    struct T_control *pT_control_selector, *pT_control_all = NULL;

    n_index = 0;
    for (kppchIndex = kstrKeys; *kppchIndex != NULL; kppchIndex++) {
        pT_control_selector = (struct T_control*)malloc(sizeof(struct T_control));
        if (pT_control_selector == NULL) {
            exit(-1);
        }
        pT_control_selector->pstr_key = *kppchIndex;
        pT_control_selector->d_control = pd_control[n_index];
        n_index++;
        HASH_ADD_KEYPTR(hh, pT_control_all, pT_control_selector->pstr_key, strlen(pT_control_selector->pstr_key), pT_control_selector);
    }

    return pT_control_all;
}

int free_pT_control(struct T_control *pT_control_all){
    struct T_control *pT_control_selector, *pT_control_tmp;
    /* free the hash table contents */
    HASH_ITER(hh, pT_control_all, pT_control_selector, pT_control_tmp) {
        HASH_DEL(pT_control_all, pT_control_selector);
        free(pT_control_selector);
    }
    return 0;
} 

double get_d_control(struct T_control *pT_control_all, char *sz_control_key)
{
    struct T_control *pT_control_selector;
    double d_control;
    HASH_FIND_STR(pT_control_all, sz_control_key, pT_control_selector);
    if (pT_control_selector != NULL) {
        d_control = pT_control_selector->d_control;
    }else{
        d_control = 0;
    }
    return d_control;
}

struct T_pwm *g_pT_pwm;
double g_arrd_pwm[4];
struct T_control *g_pT_control;
double g_arrd_control[4];
int g_arrn_ultrasound[6];/*0:up 1:down 2:left 3:right 4:forward 5:backward*/
double g_arrd_yaw_pitch_roll[3];/*0:yaw 1:pitch 2:roll*/

void ThreadTask_HTTP_get_pT_pwm(){
    while(1){
        g_pT_pwm = HTTP_get_pT_pwm();
        usleep(50000);
    }
}

void ThreadTask_get_arrd_pwm(){
    while(1){
        g_arrd_pwm[0] = get_d_pwm(g_pT_pwm, "pwm1") / 100;
        g_arrd_pwm[1] = get_d_pwm(g_pT_pwm, "pwm2") / 100;
        g_arrd_pwm[2] = get_d_pwm(g_pT_pwm, "pwm3") / 100;
        g_arrd_pwm[3] = get_d_pwm(g_pT_pwm, "pwm4") / 100;
        printf("pwm1 = %f\n", g_arrd_pwm[0]);
        printf("pwm2 = %f\n", g_arrd_pwm[1]);
        printf("pwm3 = %f\n", g_arrd_pwm[2]);
        printf("pwm4 = %f\n", g_arrd_pwm[3]);
        printf("\n");
        usleep(50000);
    }
}

void ThreadTask_Pid(){
}

void ThreadTask_update_yaw_pitch_roll(){
}

void ThreadTask_update_ultrasound(){
}

void ThreadTask_HTTP_get_control(){
}

int main()
{
    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_HTTP_get_pT_pwm, NULL);
    thpool_add_work(thpool, (void*)ThreadTask_get_arrd_pwm, NULL);
    thpool_wait(thpool);
    thpool_destroy(thpool);
    free_pT_pwm(g_pT_pwm);
    return 0;
}
