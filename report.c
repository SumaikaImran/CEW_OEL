#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define USERNAME "malaikamustafa662@gmail.com"
#define APP_PASSWORD "riii nill inxa gvmf"


/**
 * Structure to hold the upload status.
 */
struct UploadStatus {
  const char *data;
  size_t size;
};

/**
 * Callback function for reading the data to be uploaded.
 */
static size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *userp) {
  struct UploadStatus *upload_ctx = (struct UploadStatus *)userp;

  size_t to_copy = size * nmemb;
  if (to_copy > upload_ctx->size)
    to_copy = upload_ctx->size;

  if (to_copy) {
    memcpy(ptr, upload_ctx->data, to_copy);
    upload_ctx->data += to_copy;
    upload_ctx->size -= to_copy;
    return to_copy;
  }

  return 0;
}

/**
 * Main function to send an email with attachment using CURL.
 */
int main(void) {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (curl) {
    // Set email parameters
    const char *from = USERNAME;
    const char *to = "malaikamustafa662@gmail.com";
    const char *subject = "Weather Report";
    const char *body = "Hye! Here is the attached document which provides the brief report of today's weather of your city.\nStay Safe :)";

    // Construct the email payload with attachment
    char payload[4400];
    char *report_path = "report.txt";
char report_content[4096];

FILE *report_file = fopen(report_path, "r");
if (report_file != NULL) {
    size_t bytesRead = fread(report_content, 1, sizeof(report_content), report_file);
    fclose(report_file);

    if (bytesRead > 0) {
        report_content[bytesRead] = '\0';
        strcat(payload, report_content);
    }
}
   snprintf(payload, sizeof(payload),
         "From: %s\r\n"
         "To: %s\r\n"
         "Subject: %s\r\n"
         "MIME-Version: 1.0\r\n"
         "Content-Type: multipart/mixed; boundary=\"BOUNDARY\"\r\n"
         "\r\n"
         "--BOUNDARY\r\n"
         "Content-Type: text/plain\r\n"
         "\r\n"
         "%s\r\n"
         "\r\n"
         "--BOUNDARY\r\n"
         "Content-Disposition: attachment; filename=\"report.txt\"\r\n"
         "Content-Type: text/plain\r\n"
         "\r\n"
         "%s\r\n"
         "\r\n"
         "--BOUNDARY--",
         from, to, subject, body, report_content);

    struct UploadStatus upload_ctx = { payload, strlen(payload) };

    curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);

    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_USERNAME, from);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, APP_PASSWORD);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadCallback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(recipients);

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  return 0;
}