#pragma once

#include "../../http/HttpRequest.h"
#include "../../http/HttpResponse.h"
#include "../Middleware.h"
#include "CorsConfig.h"

namespace ww_http {
namespace middleware {

class CorsMiddleware : public Middleware {
public:
  explicit CorsMiddleware(
      const CorsConfig &config = CorsConfig::defaultConfig());

  void before(HttpRequest &request) override;
  void after(HttpResponse &response) override;

  std::string join(const std::vector<std::string> &strings,
                   const std::string &delimiter);

private:
  bool isOriginAllowed(const std::string &origin) const;
  void handlePreflightRequest(const HttpRequest &request,
                              HttpResponse &response);
  void addCorsHeaders(HttpResponse &response, const std::string &origin);

private:
  CorsConfig config_;
};

} // namespace middleware

} // namespace ww_http
