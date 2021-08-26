#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static int check_remote_file_exist(const char *url)
{
    CURL * curl_fd = curl_easy_init();
    CURLcode code = -1;
    CURLINFO response_code = 0;
    curl_easy_setopt(curl_fd, CURLOPT_NOBODY, 1);
    curl_easy_setopt(curl_fd, CURLOPT_TIMEOUT, 3);
    curl_easy_setopt(curl_fd, CURLOPT_URL, url);
    code = curl_easy_perform(curl_fd);

    if (code == CURLE_OK) {
        curl_easy_getinfo(curl_fd, CURLINFO_RESPONSE_CODE, &response_code);
    }

    curl_easy_cleanup(curl_fd);

    return response_code;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int lota_download(const char *url, const char *save_path)
{
    if(check_remote_file_exist(url) == 404)
    {
        printf("file not existff\n");
    }

    CURL *curl;
    FILE *fp;
    CURLcode res;
    int ret = 0;
   
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(save_path,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            ret = -1;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    
    return ret;
}