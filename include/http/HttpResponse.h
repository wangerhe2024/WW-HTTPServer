#pragma once

#include <muduo/net/TcpServer.h>

namespace ww_http {

class HttpResponse {

public:
  enum HttpStatusCode {
    wUnknown,
    w200Ok = 200,
    w204NoContent = 204,
    w301MovedPermanently = 301,
    w400BadRequest = 400,
    w401Unauthorized = 401,
    w403Forbidden = 403,
    w404NotFound = 404,
    w409Conflict = 409,
    w500InternalServerError = 500,
  };

  HttpResponse(bool close = true)
      : statusCode_(wUnknown), closeConnection_(close) {}

  void setVersion(std::string version) { httpVersion_ = version; }
  void setStatusCode(HttpStatusCode code) { statusCode_ = code; }

  HttpStatusCode getStatusCode() const { return statusCode_; }

  void setStatusMessage(const std::string message) { statusMessage_ = message; }

  void setCloseConnection(bool on) { closeConnection_ = on; }

  bool closeConnection() const { return closeConnection_; }

  void setContentType(const std::string &contentType) {
    addHeader("Content-Type", contentType);
  }

  void setContentLength(uint64_t length) {
    addHeader("Content-Length", std::to_string(length));
  }

  void addHeader(const std::string &key, const std::string &value) {
    headers_[key] = value;
  }

  void setBody(const std::string &body) {
    body_ = body;
    // body_ += "\0";
  }

  void setStatusLine(const std::string &version, HttpStatusCode statusCode,
                     const std::string &statusMessage);

  void setErrorHeader() {}

  void appendToBuffer(muduo::net::Buffer *outputBuf) const;

private:
  std::string httpVersion_;
  HttpStatusCode statusCode_;
  std::string statusMessage_;
  bool closeConnection_;
  std::map<std::string, std::string> headers_;
  std::string body_;
  bool isFile_;
};

} // namespace ww_http
