#include <stdio.h>
//libcurl (http://curl.haxx.se/libcurl/c) 
//opkg update
//opkg install libcurl
#include <curl/curl.h>

//json-c (https://github.com/json-c/json-c) 
//opkg update
//opkg install libjson
#include <json-c/json.h>
 
char* http_get(char* url)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

    /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    /* always cleanup */ 
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return 0;
}

char* http_post(char* url, char* post_data)
{
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

    /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
    /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    /* always cleanup */ 
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return 0;
}
