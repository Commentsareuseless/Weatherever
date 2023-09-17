#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

#include <mocks/MockFreeRTOS.hpp>
#include <Tasks.hpp>

// Mock like dis -> https://stackoverflow.com/questions/48659761/mocking-freertos-functions-in-gtest

struct tskTaskControlBlock {
  int dummy;
};
tskTaskControlBlock fakeTcb{69};

using ::testing::Return;
using ::testing::ReturnPointee;
using ::testing::Not;
using ::testing::NotNull;
using ::testing::IsNull;
using ::testing::StrCaseEq;

class TestAppRoot : public ::testing::Test
{
public:
  void SetUp() override {}

  void TearDown() override { mock::FreeRtosMock = nullptr; }

  void FakeInitTasks() {
    EXPECT_CALL(*mock::FreeRtosMock, xTaskCreateStatic)
        .Times(task::ID_MAX_TASKS)
        .WillRepeatedly(ReturnPointee(&fakeHandle));

    const int retval = task::InitRtosTasks();
    EXPECT_EQ(retval, 0);
  }

protected:
  TaskHandle_t fakeHandle{&fakeTcb};
  std::array<task::Task, task::ID_MAX_TASKS> taskConfig{
      {{nullptr, TaskBackground, task::Priority::BACKGROUND, {"bkg"}, {}, {}},
       {nullptr, TaskGatherData, task::Priority::HIGH, {"gdt"}, {}, {}},
       {nullptr, TaskHandleDebugCli, task::Priority::NORMAL, {"dbg"}, {}, {}}}};
};

TEST_F(TestAppRoot, CheckTaskInitNumOfCalls) {
  mock::FreeRTOS mockObject{};
  mock::FreeRtosMock = &mockObject;

  FakeInitTasks();
}

TEST_F(TestAppRoot, CheckIfInitFailsWhenOsReturnsNull) {
  int taskRetval{0};
  {
    mock::FreeRTOS mockObject{};
    mock::FreeRtosMock = &mockObject;

    // First RTOS call failed
    EXPECT_CALL(*mock::FreeRtosMock, xTaskCreateStatic)
        .Times(1)
        .WillOnce(testing::ReturnNull());
    taskRetval = task::InitRtosTasks();
    EXPECT_EQ(taskRetval, -(task::ID_TASK_BACKGROUND + 1));
  }

  {
    mock::FreeRTOS mockObject{};
    mock::FreeRtosMock = &mockObject;
    // Second RTOS call failed
    EXPECT_CALL(*mock::FreeRtosMock, xTaskCreateStatic)
        .Times(2)
        .WillOnce(ReturnPointee(&fakeHandle))
        .WillOnce(testing::ReturnNull());
    taskRetval = task::InitRtosTasks();
    EXPECT_EQ(taskRetval, -(task::ID_TASK_GATHER_DATA + 1));
  }

  {
    mock::FreeRTOS mockObject{};
    mock::FreeRtosMock = &mockObject;
    // Third RTOS call failed
    EXPECT_CALL(*mock::FreeRtosMock, xTaskCreateStatic)
        .Times(3)
        .WillOnce(ReturnPointee(&fakeHandle))
        .WillOnce(ReturnPointee(&fakeHandle))
        .WillOnce(testing::ReturnNull());

    taskRetval = task::InitRtosTasks();
    EXPECT_EQ(taskRetval, -(task::ID_TASK_HANDLE_DBG + 1));
  }
}

TEST_F(TestAppRoot, CheckArgsPassedToOs) {
  using TcbType   = decltype(taskConfig[0].tcb);
  using StackType = decltype(taskConfig[0].stack.data());

  mock::FreeRTOS mockObject{};
  mock::FreeRtosMock = &mockObject;

  auto& bkdData{taskConfig[task::ID_TASK_BACKGROUND]};
  EXPECT_CALL(*mock::FreeRtosMock,
              xTaskCreateStatic(bkdData.callback,
                                StrCaseEq(bkdData.name.data()),
                                bkdData.stack.size(), IsNull(),
                                bkdData.priority, NotNull(), NotNull()))
      .WillOnce(ReturnPointee(&fakeHandle));

  auto& ghtData{taskConfig[task::ID_TASK_GATHER_DATA]};
  EXPECT_CALL(*mock::FreeRtosMock,
              xTaskCreateStatic(ghtData.callback,
                                StrCaseEq(ghtData.name.data()),
                                ghtData.stack.size(), IsNull(),
                                ghtData.priority, NotNull(), NotNull()))
      .WillOnce(ReturnPointee(&fakeHandle));

  auto& dbgData{taskConfig[task::ID_TASK_HANDLE_DBG]};
  EXPECT_CALL(*mock::FreeRtosMock,
              xTaskCreateStatic(dbgData.callback,
                                StrCaseEq(dbgData.name.data()),
                                dbgData.stack.size(), IsNull(),
                                dbgData.priority, NotNull(), NotNull()))
      .WillOnce(ReturnPointee(&fakeHandle));

  const int retval = task::InitRtosTasks();
  EXPECT_EQ(retval, 0);
}

TEST_F(TestAppRoot, CheckGetHandleWithId) {
  const auto& taskExpectValues = taskConfig.at(task::ID_TASK_BACKGROUND);

  mock::FreeRTOS mockObject{};
  mock::FreeRtosMock = &mockObject;

  // There is no point checking what values GetTask() return
  // before initialization, because task data is staticaly allocated
  // it contains garbage from previous tests

  FakeInitTasks();

  // After init everythin should point to the same structure
  EXPECT_EQ(task::GetTask(task::ID_TASK_BACKGROUND), fakeHandle);
  EXPECT_EQ(task::GetTask(task::ID_TASK_GATHER_DATA), fakeHandle);
  EXPECT_EQ(task::GetTask(task::ID_TASK_HANDLE_DBG), fakeHandle);
}
