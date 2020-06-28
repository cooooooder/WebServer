/*
 * @Author       : mark
 * @Date         : 2020-06-15
 * @copyleft GPL 2.0
 */ 

#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>   // stat()
#include <sys/uio.h>   // readv/writev

#include <arpa/inet.h> // sockaddr_in

#include <stdio.h>  // vsnparintf()
#include <stdlib.h> //atoi()
#include <string.h>   //strcpy()
#include <stdarg.h>    // va_list


#include <errno.h>     // errno

#include "../log/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"


class HttpConn {
public:
    HttpConn();
    ~HttpConn();

    void init(int sockFd, const sockaddr_in& addr);
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);

    void Close();
    bool IsClose() const;

    int GetFd() const;
    int GetPort() const;
    const char* GetIP() const;
    sockaddr_in GetAddr() const;

    void process();
    int ToWriteBytes() { 
        return iov_[0].iov_len + iov_[1].iov_len; 
    }
    bool IsKeepAlive() const {
        return request_.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static std::atomic<int> userCount;
    
private:
    static const int PATH_LEN = 200;
    static const int READ_BUFF_SIZE = 5000;
    static const int WRITE_BUFF_SIZE = 1024;

    int fd_;
    struct  sockaddr_in addr_;

    bool isClose_;
    
    int iovCnt_;
    struct iovec iov_[2];
    
    Buffer readBuff_; // 读缓冲区
    Buffer writeBuff_; // 写缓冲区

    HttpRequest request_;
    HttpResponse response_;
};


#endif //HTTP_CONN_H