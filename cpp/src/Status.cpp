#include "Status.h"
#include <cstring>
#include <sstream>

#include <errno.h>
#include <string.h>

#include <iostream>

namespace fat {

const char * Status::copyMessage(const char * msg) {
    if(msg == nullptr) return nullptr;

    const int len = strlen(msg)+1;
    return ::strncpy(new char[len], msg, len);
}


std::string Status::toString() const {
    std::ostringstream oss;
    oss << "code: " <<  static_cast<int>(code)  << ", msg: "  <<  ((msg==nullptr)?"nullptr":msg);
    return oss.str();
}

static __thread char ThreadLocalErrMsgBuffer[1024];

std::string Status::getErrorString(const char * msg){
    std::ostringstream oss;
    strerror_r(errno, ThreadLocalErrMsgBuffer, sizeof(ThreadLocalErrMsgBuffer));
    oss << ((msg==nullptr) ? "nullptr" : msg) << ":errno-" << errno << ":" << ThreadLocalErrMsgBuffer;
    return oss.str();
}


} //end of namespace fat
