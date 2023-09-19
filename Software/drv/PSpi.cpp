/**
 * @file PSpi.cpp
 * @author Commentsareuseless
 * @brief 
 * @version 0.1
 * @date 2023-09-14
 * 
 * Copyright: Macieg :)
 * 
 */
#include "PSpi.hpp"

#include <main.h>
#include <spi.h>
#include <stm32g0xx_hal_spi.h>

namespace drv
{

void Spi::Init() { MX_SPI1_Init(); }

bool Spi::TriggerSend() { return false; }

bool Spi::TriggerRecv() { return false; }

Spi::CommStatus Spi::GetRecvStatus() { return CommStatus::FAILED; }

Spi::CommStatus Spi::GetSendStatus() { return CommStatus::FAILED; }

} // namespace drv
