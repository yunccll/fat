#include "Entry.h"

//#include <cassert>
#include <sstream>
//#include <iostream>

namespace fat {

void dateOut(std::ostream & os, uint16_t date){
    os << (date>>9) + 1980  << "-" << std::max(1, (date >> 5) & 0xf) << "-" << std::max(1, date & 0x1f);
}
void timeOut(std::ostream & os, uint16_t time){
    os << (time >> 11)  << ":" << ((time >> 5) & 0x3f) << ":" <<  ((time & 0x1f) << 1);
}
Status Entry::getName(std::string & name) const{ //for short entry(8~3)
    //FileName
    int i = 0;
    while(i < 8 && item->name[i] != (char)0x20) ++i;
    if(i == 0) {
        return Status::InvalidArgument(); //Invalid entry name
    }
    name.assign(item->name, i);

    //Suffix
    i = 0;
    while(i < 3 && item->name[8+i] != (char)0x20) ++i;
    if(i == 0)
        return Status::InvalidArgument(); //Invalid entry suffix
    name.append(".");
    name.append(item->name+8, i);
    return Status::OK();
}
std::string Entry::getCreateTimeStamp() const{
    std::ostringstream oss;
    dateOut(oss, item->createDate);
    oss << " " ;
    timeOut(oss, item->createTime);
    return oss.str();
}
std::string Entry::getLastAccessDate() const{ //read or write date
    std::ostringstream oss;
    dateOut(oss, item->lastAccessDate);
    return oss.str();
}
std::string Entry::getWriteTimeStamp() const{
    std::ostringstream oss;
    dateOut(oss, item->writeDate);
    oss << " ";
    timeOut(oss, item->writeTime);
    return oss.str();
}

std::ostream & operator<< (std::ostream & os, const Entry & entry){
    os << "Name:["              << entry.getStorageName().ToString() 
       << "] Attrbute:["        << (int)entry.getAttribute() 
       << "] CreateTimestamp:[" << entry.getCreateTimeStamp() 
       << "] lastAccessDate:["  << entry.getLastAccessDate() 
       << "] writeTimestamp:["  << entry.getWriteTimeStamp() 
       << "] firstCluster:["    << entry.getFirstCluster()
       << "] fileSize:["        << entry.getFileSize()
       << "]";
    return os;
}

} //end of namespace fat
