
#include "HttpStatus.hpp"

const HttpStatus Status::OK(200, "OK");
const HttpStatus Status::Created(201, "Created");
const HttpStatus Status::BadRequest(400, "Bad Request");
const HttpStatus Status::Forbidden(403, "Forbidden");
const HttpStatus Status::NotFound(404, "Not Found");
const HttpStatus Status::MethodNotAllowed(405, "Method Not Allowed");
const HttpStatus Status::InternalServerError(500, "Internal Server Error");
const HttpStatus Status::MovedPermanently(301, "Moved Permanently");
const HttpStatus Status::NoContent(204, "No Content");
const HttpStatus Status::Conflict(409, "Conflict");
const HttpStatus Status::PayloadTooLarge(413, "Payload Too Large");
