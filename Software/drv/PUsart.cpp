/**
 * @file PUsart.cpp
 * @author Commentsareuseless
 * @brief 
 * @date 2023-09-12
 * 
 * Copyright: Macieg :)
 * 
 */
#include "PUsart.hpp"

#include <main.h>
#include <stm32g0xx_hal_uart.h>

namespace drv
{

void Usart::Init() {}

bool Usart::TriggerSend() { return false; }

bool Usart::TriggerRecv() { return false; }

Usart::CommStatus Usart::GetRecvStatus() { return CommStatus::FAILED; }

Usart::CommStatus Usart::GetSendStatus() { return CommStatus::FAILED; }

} // namespace drv
