#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP
#include <string>

class HttpStatus {
public:
    int code;
    std::string reason;
    HttpStatus(int c, const std::string& r): code(c), reason(r) {}
};

class Status {
public:
    static const HttpStatus OK;
    static const HttpStatus Created;
    static const HttpStatus BadRequest;
    static const HttpStatus Forbidden;
    static const HttpStatus NotFound;
    static const HttpStatus MethodNotAllowed;
    static const HttpStatus InternalServerError;
    static const HttpStatus MovedPermanently;
    static const HttpStatus NoContent;
    static const HttpStatus Conflict;
    static const HttpStatus PayloadTooLarge;
};

#endif // HTTPSTATUS_HPP
