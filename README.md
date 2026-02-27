# webserv

A non-blocking HTTP/1.1 web server written in C++98 from scratch using raw POSIX sockets and `select()`.

## Features

- Non-blocking I/O via `select()`
- Multiple virtual servers on different ports
- HTTP methods: `GET`, `POST`, `DELETE`
- CGI support (PHP, Python)
- File uploads
- Directory autoindex
- Custom error pages
- Nginx-style config file parsing

## Build

```bash
make
```

## Usage

```bash
./webserv [config_file]
# defaults to configs/default.conf
```

## Config

```nginx
http {
    server {
        listen 9000;
        server_name localhost;
        root www;
        client_max_body_size 10485760;

        location /uploads {
            limit_except GET POST DELETE;
            upload_enable on;
            autoindex on;
        }

        location /cgi-bin {
            cgi_pass .php /usr/bin/php-cgi;
            cgi_pass .py /usr/bin/python3;
        }
    }
}
```

## Structure

```
srcs/
  main.cpp
  ServerAndClient/   # socket init, select loop, client I/O
  ReqRes/            # request parsing, response building, method handlers
  ParsingAndCgi/     # config lexer/parser/AST, CGI execution
```
