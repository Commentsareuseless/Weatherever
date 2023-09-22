#include <cstdint>
#include <etl/array.h>
#include <gtest/gtest.h>

// The neat trick to test protected methods
#define private   public
#define protected public
#include <CommHandler.hpp>
#undef protected
#undef private

template <typename DataType, uint32_t DataSize>
constexpr etl::array<DataType, DataSize> initArray(DataType value) {
  etl::array<DataType, DataSize> dataToRet{};
  dataToRet.fill(value);
  return dataToRet;
}

constexpr uint32_t BUFF_SIZE{256};
constexpr uint INVALID_VALUE{0xAA};
constexpr etl::array<uint8_t, BUFF_SIZE> TEST_DATA{
    initArray<uint8_t, BUFF_SIZE>(INVALID_VALUE)};

class TestCommHandler : public ::testing::Test
{
public:
  void SetUp() override {
    TestInstance.flushRx();
    TestInstance.flushTx();
  }
  void TearDown() override {}

  drv::CommHandler<BUFF_SIZE, BUFF_SIZE> TestInstance;
};

TEST_F(TestCommHandler, WriteToBuff) {}
