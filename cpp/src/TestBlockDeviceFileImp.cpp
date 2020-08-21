#include "gtest/gtest.h"

#include <memory>
#include "Device.h"

using namespace fat;

TEST(BlockDeviceFileImp, use){
    auto device = std::make_shared<BlockDeviceFileImp>("floppy.img");

    std::string buf;
    ASSERT_TRUE(device->open().isOk());
    ASSERT_TRUE(device->read((void*)0, buf).isOk());
    ASSERT_TRUE(buf.size() == device->getBlockSize());

    ASSERT_TRUE(device->write((void*)0, Slice(buf)).isOk());

    ASSERT_TRUE(device->close().isOk());
}

