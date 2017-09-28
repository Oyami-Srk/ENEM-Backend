#include "Netease.h"
#include <cstdio>
#include <curl/curl.h>

using ENEM::HTTP;
using ENEM::Netease;
using std::string;

static int OnDebug(CURL *, curl_infotype itype, char *pData, size_t size,
                   void *) {
  if (itype == CURLINFO_TEXT) {
    printf("[TEXT]%s\n", pData);
  } else if (itype == CURLINFO_HEADER_IN) {
    printf("[HEADER_IN]%s\n", pData);
  } else if (itype == CURLINFO_HEADER_OUT) {
    printf("[HEADER_OUT]%s\n", pData);
  } else if (itype == CURLINFO_DATA_IN) {
    printf("[DATA_IN]%s\n", pData);
  } else if (itype == CURLINFO_DATA_OUT) {
    printf("[DATA_OUT]%s\n", pData);
  }
  return 0;
}

static size_t OnWriteData(void *buffer, size_t size, size_t nmemb,
                          void *lpVoid) {
  std::string *str = dynamic_cast<std::string *>((std::string *)lpVoid);
  if (NULL == str || NULL == buffer) {
    return -1;
  }

  char *pData = (char *)buffer;
  str->append(pData, size * nmemb);
  return nmemb;
}

int HTTP::Post(const string &url, const string &post, string &resp) {
  CURLcode res;
  CURL *curl = curl_easy_init();
  if (NULL == curl) {
    return CURLE_FAILED_INIT;
  }
  if (__debug_flag__) {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
  }
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
  res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return res;
}

int HTTP::Get(const std::string &url, std::string &resp) {
  CURLcode res;
  CURL *curl = curl_easy_init();
  if (NULL == curl) {
    return CURLE_FAILED_INIT;
  }
  if (__debug_flag__) {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
  }
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
  /**
   * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
   * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
   */
  curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
  res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  return res;
}

/*
 * From
 *      CSDN
 */

