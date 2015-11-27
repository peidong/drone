#include <stdio.h>
//libcurl (http://curl.haxx.se/libcurl/c)
//opkg update
//opkg install libcurl
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/* the function to invoke as the data recieved */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    char **response_ptr =  (char**)userp;
    *response_ptr = strndup(contents, (size_t)(size *nmemb));
    return strlen(*response_ptr);
}

char* http_get_new(char* sz_url) {
    CURL *curl_handle;
    CURLcode res;
    char *sz_response = NULL;

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, sz_url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
        // fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        sz_response = chunk.memory;
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return sz_response;
}
/**
 * @param the http get url
 * @return the response
 */
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
        //curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

    /* follow locations specified by the response header */
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    /* setting a callback function to return the data */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    /* passing the pointer to the response as the callback parameter */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

    /* Check for errors */
        if(res != CURLE_OK){
            // fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
    /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return response;
}
/**
 * @param url
 * @param post data
 * @return response
 */
char* http_post(char* url, char* post_data)
{
    CURL *curl;
    CURLcode res;
    char *response=NULL;

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

    /* setting a callback function to return the data */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    /* passing the pointer to the response as the callback parameter */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
    /* Check for errors */
        if(res != CURLE_OK){
            // fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

    /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return response;
}