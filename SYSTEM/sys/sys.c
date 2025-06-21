#include "sys.h"

__attribute__((naked)) void WFI_SET(void)
{
	__asm("wfi");
}
//disable most of int
__attribute__((naked)) void INTX_DISABLE(void) {
    __asm volatile ("CPSID I");
    __asm volatile ("BX LR");
}
//���������ж�
__attribute__((naked)) void INTX_ENABLE(void)
{
	__asm volatile ("CPSIE I");
	__asm volatile ("BX LR");
}
//����ջ����ַ
//addr:ջ����ַ
__attribute__((naked)) void MSR_MSP(u32 addr)
{
	__asm("MSR MSP, r0"); 			//set Main Stack value
	__asm("BX r14");

}
















