#pragma once

#include "HttpRequest.h"
#include <iostream>
#include <muduo/net/TcpServer.h>

namespace ww_http {
class HttpContext {
public:
  enum HttpRequestParseState {
    wExpectRequestLine, // 解析请求行
    wExpectHeaders,     // 解析请求头
    wExpectBody,        // 解析请求体
    wGotAll,            // 解析完成
  };

  HttpContext() : state_(wExpectRequestLine) {}

  bool parseRequest(muduo::net::Buffer *buf, muduo::Timestamp receiveTime);

  bool gotAll() const { return state_ == wGotAll; }

  void reset() {
    state_ = wExpectRequestLine;
    HttpRequest dummyDate;
    request_.swap(dummyDate);
  }

  const HttpRequest &request() const { return request_; }

  HttpRequest &request() { return request_; }

private:
  bool processRequestLine(const char *begin, const char *end);

private:
  HttpRequestParseState state_;
  HttpRequest request_;
};

} // namespace ww_http