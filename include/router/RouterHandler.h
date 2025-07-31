#pragma once
#include "../http/HttpRequest.h"
#include "../http/HttpResponse.h"
#include <memory>
#include <string>

namespace ww_http {
namespace router {

class RouterHandler {
public:
  virtual ~RouterHandler() = default;
  virtual void handle(const HttpRequest &req, HttpResponse *resp) = 0;
};

} // namespace router
} // namespace ww_http