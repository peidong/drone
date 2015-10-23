#include "http/http.h"
#include <json-c/json.h>

int main(void)
{   struct json_object *jobj; 
    char* content;
    struct json_object *pwm1, *data, *pwm2, *pwm3, *pwm4, *update_time;
    int res1, res2;

    printf("\nStart HTTP GET");
    content = http_get("http://fryer.ee.ucla.edu/rest/api/pwm/get/");
    //printf("\n%s", content);
    jobj = json_tokener_parse(content);
    printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));
    res1 = json_object_object_get_ex(jobj,"data",&data);
    res2 = json_object_object_get_ex(data,"pwm1",&pwm1);
    res2 = json_object_object_get_ex(data,"pwm2",&pwm2);
    res2 = json_object_object_get_ex(data,"pwm3",&pwm3);
    res2 = json_object_object_get_ex(data,"pwm4",&pwm4);
    res2 = json_object_object_get_ex(data,"update_time",&update_time);

    printf(" pwm1 is: %f\n pwm2 is: %f\n pwm3 is: %f\n pwm4 is: %f\n",json_object_get_double(pwm1),json_object_get_double(pwm2),json_object_get_double(pwm3),json_object_get_double(pwm4));
    printf("update time is: %s\n", json_object_get_string(update_time));

    printf("\n\nStart HTTP POST");
    content = http_post("http://fryer.ee.ucla.edu/rest/api/pwm/post/", "pwm1=1.23&pwm2=2.34&pwm3=3.45&pwm4=4.56");
    printf("\n%s", content);
    return 0;
}
