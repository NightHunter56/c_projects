#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define API_URL "https://results.digilocker.gov.in/api/cbse/hscer/results"
#define TOTAL_SUBJECTS 6

struct Memory {
    char *response;
    size_t size;
};

static size_t write_callback(void *contents,
                             size_t size,
                             size_t nmemb,
                             void *userp)
{
    size_t realsize = size * nmemb;

    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response,
                        mem->size + realsize + 1);

    if(ptr == NULL)
        return 0;

    mem->response = ptr;

    memcpy(&(mem->response[mem->size]),
           contents,
           realsize);

    mem->size += realsize;

    mem->response[mem->size] = '\0';

    return realsize;
}

void print_subjects(cJSON *data)
{
    printf("\n================================ SUBJECTS ================================\n");

    printf("%-10s %-30s %-10s %-10s %-10s %-10s\n",
           "CODE",
           "SUBJECT",
           "THEORY",
           "PRACT",
           "TOTAL",
           "GRADE");

    printf("----------------------------------------------------------------------------\n");

    for(int i = 1; i <= TOTAL_SUBJECTS; i++)
    {
        char key[32];

        sprintf(key, "SUB%d", i);

        cJSON *sub = cJSON_GetObjectItem(data, key);

        if(!cJSON_IsString(sub))
            continue;

        char sname_key[32];
        char th_key[32];
        char pr_key[32];
        char total_key[32];
        char grade_key[32];

        sprintf(sname_key, "SNAME%d", i);
        sprintf(th_key, "MRK%d1", i);
        sprintf(pr_key, "MRK%d2", i);
        sprintf(total_key, "MRK%d3", i);
        sprintf(grade_key, "GR%d", i);

        cJSON *sname = cJSON_GetObjectItem(data, sname_key);
        cJSON *theory = cJSON_GetObjectItem(data, th_key);
        cJSON *practical = cJSON_GetObjectItem(data, pr_key);
        cJSON *total = cJSON_GetObjectItem(data, total_key);
        cJSON *grade = cJSON_GetObjectItem(data, grade_key);

        printf("%-10s %-30s %-10s %-10s %-10s %-10s\n",
               sub->valuestring,
               sname ? sname->valuestring : "-",
               theory ? theory->valuestring : "-",
               practical ? practical->valuestring : "-",
               total ? total->valuestring : "-",
               grade ? grade->valuestring : "-");
    }

    printf("============================================================================\n");
    exit(1);
}

void fetch_and_print_result(const char *roll,
                            const char *admit)
{
    CURL *curl;
    CURLcode res;

    struct Memory chunk;

    chunk.response = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();

    if(!curl)
    {
        printf("Failed to initialize curl\n");
        return;
    }

    char postfields[256];

    snprintf(postfields,
             sizeof(postfields),
             "rroll=%s&year=2026&admn_id=%s",
             roll,
             admit);

    curl_easy_setopt(curl, CURLOPT_URL, API_URL);

    curl_easy_setopt(curl,
                     CURLOPT_POSTFIELDS,
                     postfields);

    curl_easy_setopt(curl,
                     CURLOPT_WRITEFUNCTION,
                     write_callback);

    curl_easy_setopt(curl,
                     CURLOPT_WRITEDATA,
                     (void *)&chunk);

    curl_easy_setopt(curl,
                     CURLOPT_USERAGENT,
                     "Mozilla/5.0");

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        fprintf(stderr,
                "\nRequest Failed: %s\n",
                curl_easy_strerror(res));

        curl_easy_cleanup(curl);

        free(chunk.response);

        return;
    }

    cJSON *json = cJSON_Parse(chunk.response);

    if(!json)
    {
        printf("\nFailed to parse JSON\n");

        curl_easy_cleanup(curl);

        free(chunk.response);

        return;
    }

    cJSON *data = cJSON_GetObjectItem(json, "data");

    if(!cJSON_IsObject(data))
    {
        printf("\nNo result found for Roll: %s\n",
               roll);

        cJSON_Delete(json);

        curl_easy_cleanup(curl);

        free(chunk.response);

        return;
    }

    printf("\n\n========================================================================\n");
    printf("                            CBSE RESULT                                 \n");
    printf("========================================================================\n");

    cJSON *name   = cJSON_GetObjectItem(data, "CNAME");
    cJSON *rollno = cJSON_GetObjectItem(data, "RROLL");
    cJSON *fname  = cJSON_GetObjectItem(data, "FNAME");
    cJSON *mname  = cJSON_GetObjectItem(data, "MNAME");
    cJSON *school = cJSON_GetObjectItem(data, "SCH_NAME");
    cJSON *classs = cJSON_GetObjectItem(data, "CLASS");
    cJSON *result = cJSON_GetObjectItem(data, "RES");

    printf("Name         : %s\n",
           name ? name->valuestring : "-");

    printf("Roll Number  : %s\n",
           rollno ? rollno->valuestring : "-");

    printf("Father Name  : %s\n",
           fname ? fname->valuestring : "-");

    printf("Mother Name  : %s\n",
           mname ? mname->valuestring : "-");

    printf("School       : %s\n",
           school ? school->valuestring : "-");

    printf("Class        : %s\n",
           classs ? classs->valuestring : "-");

    printf("Result       : %s\n",
           result ? result->valuestring : "-");

    print_subjects(data);

    cJSON_Delete(json);

    curl_easy_cleanup(curl);

    free(chunk.response);
}

int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
	char roll[32];
	char admit[32];
	for(int i=21; i<=99;i++){
		sprintf(roll,"206260%d",i);
		sprintf(admit,"IN%d5571",i);
		printf("%s\n",admit);
		fetch_and_print_result(roll,admit);
		
	}
    
	    curl_global_cleanup();

    return 0;
}
