#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <curl/curl.h>
#include<json-c/json.h>





int main(void)
{
    CURL* curl;
    FILE* fp;
    CURLcode res;

	char category[11];
	char url[60] = "https://imdb-api.com/en/API/Keyword/k_fro8k5jd/";
	char buffer[15000];
	char strYear[15];

	struct json_object* parsed_json;
	struct json_object* parsed_json2;
	struct json_object* keyword;
	struct json_object* title;
	struct json_object* year;
	struct json_object* rating;
	struct json_object* items;
	struct json_object* shows;

	int z = 0;
	int y = 1;
	int tamanho = 0;
	size_t n_shows;
	size_t i;


	printf("what type/category of shows are you searching? -> ");
	scanf("%s", &category);

	strcat(url, category);

    curl = curl_easy_init();
    if (curl) {
		fp = fopen("example.txt", "wb");
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL,url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        res = curl_easy_perform(curl);
		fclose(fp);

		if (res != CURLE_OK) {
			fprintf(stderr, "resquest failed %s\n", curl_easy_strerror(res));
		}
		else {
			fp = fopen("example.txt", "r");
			if (fp == NULL)
			{
				printf("Erro na abertura do arquivo!");
				return 1;
			}

			fread(buffer, 15000, 1, fp);
			fclose(fp);

			parsed_json = json_tokener_parse(buffer);
			parsed_json2 = json_tokener_parse(buffer);

			json_object_object_get_ex(parsed_json, "keyword", &keyword);
			json_object_object_get_ex(parsed_json, "items", &items);

			n_shows = json_object_array_length(items);
			printf("Found %zu shows related to %s\n", n_shows, json_object_get_string(keyword));
			
			if (n_shows == 0) {
				printf("Try again with others categories. Some easy ones are -> love, drama, anime, hollywood, comedy...\n");
				main();
			}

			for (i = 0; i < n_shows; i++) {
				shows = json_object_array_get_idx(items, i);
				parsed_json2 = json_object_array_get_idx(items, i);

				json_object_object_get_ex(parsed_json2, "title", &title);
				json_object_object_get_ex(parsed_json2, "year", &year);
				json_object_object_get_ex(parsed_json2, "imDbRating", &rating);

				strcpy(strYear, json_object_get_string(year));
				tamanho = strlen(strYear);
				for (y = 1; y <= tamanho - 2; y++) {
					if (strYear[y] != '2' && strYear[y] != '0' && strYear[y] != '1' && strYear[y] != '3' && strYear[y] != '4' && strYear[y] != '5' && strYear[y] != '6' && strYear[y] != '7' && strYear[y] != '8' && strYear[y] != '9') {//achar jeito aqui
						strYear[y] = '-';
					}
				}

				printf("%zu. %s %s Nota: %s\n ", i + 1, json_object_get_string(title), strYear, json_object_get_string(rating));
			}
		}
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}