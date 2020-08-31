#pragma once

#include <string>

namespace fat {

class Status {
public:
    enum ErrorCode : unsigned char {
        ecOk=0,
        ecFailed=1,
        ecInvalidArgument,
        ecNotSupported,
        ecEndOfFile,
        ecTimeout,
        ecNotFound,
        ecAborted,
        ecIOError,
        ecErrorFormat,
        ecFileNotOpen,
        ecMaxCode,
    };

    Status(): code(ecOk), msg(nullptr){}
    ~Status(){ if(msg != nullptr) delete [] msg;}


    //  ***************************   static 
    static Status OK(const char * msg = nullptr){
        return Status(ErrorCode::ecOk, msg);
    }
    static Status Failed(const char * msg = nullptr){
        return Status(ErrorCode::ecFailed, msg);
    }
    static Status InvalidArgument(const char * msg = nullptr){
        return Status(ErrorCode::ecInvalidArgument, msg);
    }
    static Status NotSupported(const char * msg = nullptr){
        return Status(ErrorCode::ecNotSupported, msg);
    }
    static Status EndOfFile(){
        return Status(ErrorCode::ecEndOfFile, "end of file");
    }
    static Status Timeout(const char * msg = nullptr){
        return Status(ErrorCode::ecTimeout, msg);
    }
    static Status NotFound(const char * msg = nullptr){
        return Status(ErrorCode::ecNotFound, msg);
    }
    static Status Aborted(const char * msg = nullptr){
        return Status(ErrorCode::ecAborted, msg);
    }
    static Status IOError(const char * msg = nullptr){
        return Status(ErrorCode::ecIOError, msg);
    }
    static Status IOErrorWithErrno(const char * msg = nullptr){
        std::string str = getErrorString(msg);
        return Status(ErrorCode::ecIOError, str.data());
    }
    static Status ErrorFormat(const char * msg = nullptr){
        return Status(ErrorCode::ecErrorFormat, msg);
    }
    static Status FileNotOpen(const char * msg = nullptr){
        return Status(ErrorCode::ecFileNotOpen, msg);
    }
    //TODO:
    //  ***************************   isXXX
    bool isOk() const { return code == ErrorCode::ecOk;}
    bool isFailed() const { return code == ErrorCode::ecFailed;}
    bool isInvalidArgument() const { return code == ErrorCode::ecInvalidArgument;}
    bool isNotSupported() const { return code == ErrorCode::ecNotSupported;}
    bool isEndOfFile() const { return code == ErrorCode::ecEndOfFile;}
    bool isTimeout() const { return code == ErrorCode::ecTimeout;}
    bool isNotFound() const { return code == ErrorCode::ecNotFound;}
    bool isAborted() const { return code == ErrorCode::ecAborted;}
    bool isIOError() const { return code == ErrorCode::ecIOError;}
    bool isErrorFormat() const { return code == ErrorCode::ecErrorFormat;}
    bool isFileNotOpen() const { return code == ErrorCode::ecFileNotOpen;}
    //TODO:

    //copy
    Status(const Status  & st);
    Status(Status && st);
    //assign
    Status & operator=(const Status & st);
    Status & operator=(Status && st);
    //Compare
    bool operator==(const Status& rhs) const;
    bool operator!=(const Status& rhs) const;

    operator bool() const {
        return isOk();
    }

    //For debug
    std::string toString() const;

    ErrorCode Code() const { return code;}
    const char * Message() const { return msg; }

protected:
    explicit Status(ErrorCode code, const char * msg = nullptr);
    static const char * copyMessage(const char * msg);
private:
    static std::string getErrorString(const char * msg);
    ErrorCode code;
    const char * msg;
};

inline Status::Status(ErrorCode code, const char * msg)
: code(code)
, msg(copyMessage(msg))
{
}

inline Status::Status(const Status  & st)
: Status(st.code, copyMessage(st.msg))
{
}
inline Status::Status(Status && st)
: Status()
{
    *this = std::move(st);
}

inline Status & Status::operator=(Status && st){
    if(this != &st){
        code = std::move(st.code);
        st.code = ecOk;
        
        if(msg != nullptr) delete [] msg; 
        msg = nullptr;
        std::swap(msg, st.msg);
    }
    return *this;
}
inline Status & Status::operator=(const Status & st){
    if(this != &st){
        code = st.code;

        if(msg != nullptr) delete [] msg;
        msg = copyMessage(st.msg);
    }
    return *this;
}
inline bool Status::operator==(const Status& rhs) const{
    return code == rhs.code;
}
inline bool Status::operator!=(const Status& rhs) const{
    return !this->operator==(rhs);
}
} //end of namespace fat
