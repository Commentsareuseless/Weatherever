/**
 * @file PI2C.cpp
 * @author Commentsareuseless
 * @brief 
 * @version 0.1
 * @date 2023-09-14
 * 
 * Copyright: Macieg :)
 * 
 */
#include "PI2C.hpp"

#include <main.h>
#include <i2c.h>
#include <stm32g0xx_hal_i2c.h>

namespace drv
{

void I2C::Init() {}

bool I2C::TriggerSend() { return false; }

bool I2C::TriggerRecv() { return false; }

I2C::CommStatus I2C::GetRecvStatus() { return CommStatus::FAILED; }

I2C::CommStatus I2C::GetSendStatus() { return CommStatus::FAILED; }

} // namespace drv
