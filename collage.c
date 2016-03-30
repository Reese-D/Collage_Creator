#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdlib.h>

//compile with -lcurl flag

size_t writeFunction(void *buffer, size_t size, size_t nmemb, void* userP)
{
    FILE* stream = (FILE*)userP;
    if (!stream){
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)buffer, size, nmemb, stream);
    return written;
}

int download_jpeg(char* url)
{
    FILE* fp = fopen("out.jpg", "wb");
    if (!fp){
        printf("Failed to create file on the disk\n");
        return 0;
    }

    char* errorBuffer = malloc(sizeof(char) * CURL_ERROR_SIZE);
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    // curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);

    // #ifdef DEBUG
    //   curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
    //   curl_easy_setopt(handle, CURLOPT_HEADER, 1);
    // #endif

    CURLcode success = curl_easy_perform(handle);
    if (success){
        printf("invalid URL, failed to download: %s\n", url);
        return 0;
    }

    long res_code = 0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_code);
    if (!((res_code == 200 || res_code == 201) && success != CURLE_ABORTED_BY_CALLBACK)){
        printf("Response code: %lu\n", res_code);
        printf("Error buffer: %s\n", errorBuffer);
        return 0;
    }

    curl_easy_cleanup(handle);
    fclose(fp);

    return 1;
}

int main(int argc, char** argv)
{
    //TODO read from output file urls, will have to parse json
    if (argc < 2){
       printf("Usage: %s <url>\n", argv[0]);
       return -1;
    }

    if (!download_jpeg(argv[1])){
        printf("!! Failed to download file: %s\n", argv[1]);
        return -1;
    }

    return 0;
}
