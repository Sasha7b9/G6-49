#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef * /*RCC_ClkInitStruct*/, uint32_t /*FLatency*/)
{
    return HAL_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef * /*RCC_OscInitStruct*/)
{
    return HAL_ERROR;
}