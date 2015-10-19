#include <stdio.h>
//libcurl (http://curl.haxx.se/libcurl/c) 
//opkg update
//opkg install libcurl
#include <curl/curl.h>
#include <curl/easy.h>
#include <string.h>

/* the function to invoke as the data recieved */
size_t static write_callback_func(void *buffer, size_t size, size_t nmemb, void *userp)
{
    char **response_ptr =  (char**)userp;

    /* assuming the response is a string */
    *response_ptr = strndup(buffer, (size_t)(size *nmemb));
    return 0;
}


char* http_get(char* url)
{
    CURL *curl;
    CURLcode res;
    char *response=NULL;

    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

    /* follow locations specified by the response header */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    /* setting a callback function to return the data */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_func);

    /* passing the pointer to the response as the callback parameter */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

    /* Check for errors */ 
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
    /* always cleanup */ 
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return response;
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
