#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include <muduo/base/Timestamp.h>

namespace ww_hppt {

class HttpRequest {

public:
  enum Method { wIncalid, wGet, wPost, wHead, wPut, wDelete, wOptions };

  HttpRequest() : method_(wIncalid), version_("Unkonwn") {}

  void setReceiveTime(muduo::Timestamp t);

  muduo::Timestamp receiveTime() const { return receiveTime_; }

  bool setMethod(const char *start, const char *end);

  std::string path() const { return path_; }

  void setPathParameters(const std::string &key, const std::string &value);

  std::string getPathParamters(const std::string &key) const;

  void setQueryParameters(const std::string &key, const std::string &value);

  std::string getQueryparameters(const std::string &key) const;

  void setVersion(std::string v) { version_ = v; }

  void addHeader(const char *start, const char *colon, const char *end);

  std::string getHeader(const std::string &field) const;

  const std::map<std::string, std::string> &headers() const { return headers_; }

  void setBody(const std::string &body) { content_ = body; }
  void setBody(const char *start, const char *end) {
    if (end >= start) {
      content_.assign(start, end - start);
    }
  }

  std::string getBody() const { return content_; }

  void setContnenLength(uint64_t length) { contentLength_ = length; }

  uint64_t getContentLength() const { return contentLength_; }

  void swap(HttpRequest &thar);

private:
  Method method_;                                                // 请求方法
  std::string version_;                                          // http版本
  std::string path_;                                             //请求路径
  std::unordered_map<std::string, std::string> pahtParameters_;  //路径参数
  std::unordered_map<std::string, std::string> queryParemeters_; //查询参数
  muduo::Timestamp receiveTime_;                                 //接收时间
  std::map<std::string, std::string> headers_;                   //请求头
  std::string content_;                                          //请求体
  uint64_t contentLength_{0}; // 求情体长度
};

} // namespace ww_hppt