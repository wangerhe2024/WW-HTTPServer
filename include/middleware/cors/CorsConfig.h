#pragma once

#include <string>
#include <vector>

namespace ww_http {
namespace middleware {

struct CorsConfig {

  std::vector<std::string> allowedOrigins;
  std::vector<std::string> allowedMethods;
  std::vector<std::string> allowedHeaders;

  bool allowCredentials = false;
  int maxAge = 3600;

  static CorsConfig defaultConfig() {
    CorsConfig config;
    config.allowedOrigins = {"*"};
    config.allowedMethods = {"GET", "POST", "PUT", "DELETE", "OPTIONS"};
    config.allowedHeaders = {"Content-Type", "Authorization"};
    return config;
  }
};

} // namespace middleware

} // namespace ww_http
