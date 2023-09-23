#include <gtest/gtest.h>
#include <cstdint>
#include <array>

// The neat trick to test protected methods
#define private   public
#define protected public
#include <CommHandler.hpp>
#undef protected
#undef private

template <typename DataType, uint32_t DataSize>
constexpr std::array<DataType, DataSize> initArray(DataType value) {
  std::array<DataType, DataSize> dataToRet{};

  for (auto& elem : dataToRet) { elem = value; }

  return dataToRet;
}

template <typename DataType, uint32_t DataSize>
constexpr std::array<DataType, DataSize> initArraySeq(DataType startValue) {
  static_assert(std::is_integral_v<DataType>,
                "initArraySeq() works only with integral types!");
  std::array<DataType, DataSize> dataToRet{};
  DataType iter{0};

  for (auto& elem : dataToRet) {
    elem = startValue + iter;
    ++iter;
  }

  return dataToRet;
}

constexpr uint32_t BUFF_SIZE{256};
constexpr uint8_t INVALID_VALUE{0xAA};

constexpr std::array<uint8_t, BUFF_SIZE> TEST_DATA_VALID{
    initArray<uint8_t, BUFF_SIZE>(INVALID_VALUE)};

constexpr std::array<uint8_t, BUFF_SIZE + 1> TEST_DATA_TOO_LONG{
    initArray<uint8_t, BUFF_SIZE + 1>(INVALID_VALUE)};

constexpr std::array<uint8_t, BUFF_SIZE> TEST_DATA_SEQUENCIAL{
    initArraySeq<uint8_t, BUFF_SIZE>(1)};

class TestCommHandler : public ::testing::Test
{
public:
  void SetUp() override {
    TestInstance.flushRx();
    TestInstance.flushTx();
  }
  void TearDown() override {}

  template <typename Iter, typename DataType>
  void VerifyContents(Iter begin, Iter end, const DataType expected) {
    static_assert(std::is_integral_v<DataType>, "Data must be integral type!");

    for (Iter idx{begin}; idx != end; ++idx) { EXPECT_EQ(*idx, expected); }
  }

  template <typename Iter, typename DataType, size_t dataSize>
  void VerifyContentsWithContainer(
      Iter begin,
      Iter end,
      const std::array<DataType, dataSize>& expected) {
    static_assert(std::is_integral_v<DataType>, "Data must be integral type!");

    ASSERT_EQ(static_cast<size_t>(end - begin), expected.size());

    Iter idx{begin};

    for (auto val : expected) {
      EXPECT_EQ(val, *idx);
      ++idx;
    }
  }

  drv::CommHandler<BUFF_SIZE, BUFF_SIZE> TestInstance;
};

TEST_F(TestCommHandler, FlushBuffs) {
  // Flush happens in SetUp()
  VerifyContents(TestInstance.recvBuff.begin(), TestInstance.recvBuff.end(),
                 0x00u);
  VerifyContents(TestInstance.sendBuff.begin(), TestInstance.sendBuff.end(),
                 0x00u);
}

TEST_F(TestCommHandler, NormalWriteToBuff) {
  size_t retval{0};

  retval =
      TestInstance.pushToRx(TEST_DATA_VALID.data(), TEST_DATA_VALID.size());
  EXPECT_EQ(retval, TEST_DATA_VALID.size());
  VerifyContents(TestInstance.recvBuff.begin(), TestInstance.recvBuff.end(),
                 INVALID_VALUE);

  retval = 0;
  retval =
      TestInstance.pushToTx(TEST_DATA_VALID.data(), TEST_DATA_VALID.size());
  EXPECT_EQ(retval, TEST_DATA_VALID.size());
  VerifyContents(TestInstance.sendBuff.begin(), TestInstance.sendBuff.end(),
                 INVALID_VALUE);

  retval = 0;
  retval = TestInstance.pushToRx(TEST_DATA_SEQUENCIAL.data(),
                                 TEST_DATA_SEQUENCIAL.size());
  EXPECT_EQ(retval, TEST_DATA_SEQUENCIAL.size());
  VerifyContentsWithContainer(TestInstance.recvBuff.begin(),
                              TestInstance.recvBuff.end(),
                              TEST_DATA_SEQUENCIAL);

  retval = 0;
  retval = TestInstance.pushToTx(TEST_DATA_SEQUENCIAL.data(),
                                 TEST_DATA_SEQUENCIAL.size());
  EXPECT_EQ(retval, TEST_DATA_SEQUENCIAL.size());
  VerifyContentsWithContainer(TestInstance.sendBuff.begin(),
                              TestInstance.sendBuff.end(),
                              TEST_DATA_SEQUENCIAL);
}

TEST_F(TestCommHandler, NormalReadFromRecv) {
  constexpr uint8_t RECV_DATA{0xCCu};
  size_t retval{0};
  std::array<uint8_t, BUFF_SIZE> writeBuff{};
  writeBuff.fill(RECV_DATA);

  retval = TestInstance.pushToRx(writeBuff.data(), writeBuff.size());
  EXPECT_EQ(retval, writeBuff.size());
  VerifyContents(TestInstance.recvBuff.begin(), TestInstance.recvBuff.end(),
                 RECV_DATA);

  // Make sure buff data has changed
  writeBuff.fill(INVALID_VALUE);

  retval = 0;
  retval = TestInstance.popFromRx(writeBuff.data(), writeBuff.size());
  EXPECT_EQ(retval, writeBuff.size());
  // We poped all data so buff should be empty
  EXPECT_EQ(true, TestInstance.recvBuff.empty());
  EXPECT_EQ(true, TestInstance.canReceiveData());

  VerifyContents(writeBuff.begin(), writeBuff.end(), RECV_DATA);
}

TEST_F(TestCommHandler, NormalReadFromSend) {
  constexpr uint8_t RECV_DATA{0xCCu};
  size_t retval{0};
  std::array<uint8_t, BUFF_SIZE> writeBuff{};
  writeBuff.fill(RECV_DATA);

  retval = TestInstance.pushToRx(writeBuff.data(), writeBuff.size());
  EXPECT_EQ(retval, writeBuff.size());
  VerifyContents(TestInstance.sendBuff.begin(), TestInstance.sendBuff.end(),
                 RECV_DATA);

  // Make sure buff data has changed
  writeBuff.fill(INVALID_VALUE);

  retval = TestInstance.popFromRx(writeBuff.data(), writeBuff.size());
  EXPECT_EQ(retval, writeBuff.size());
  // We poped all data so buff should be empty
  EXPECT_EQ(true, TestInstance.sendBuff.empty());
  EXPECT_EQ(false, TestInstance.hasDataToSend());

  VerifyContents(writeBuff.begin(), writeBuff.end(), RECV_DATA);
}

TEST_F(TestCommHandler, WriteTooMuchData) {
  // At this point, there is no use of testing both Rx and Tx variant
  // so we stick to Tx

  constexpr uint8_t LAST_VALUE_MARK{0xDD};
  auto writeBuff{TEST_DATA_TOO_LONG};
  size_t retval{0};

  // Make sure last value is different than the rest
  // so we can later check if it was written and not
  // trunctuated as expected
  writeBuff.back() = LAST_VALUE_MARK;

  retval = TestInstance.pushToTx(writeBuff.data(), writeBuff.size());
  // Pushed data should be trunctuated
  EXPECT_EQ(retval, TestInstance.txSize());
  EXPECT_NE(LAST_VALUE_MARK, TestInstance.sendBuff.back());
}