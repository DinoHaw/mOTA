/**
 * ���ļ����ڹر� semihosting
 */
#include "common.h"

/* ���߱�������û��ʹ�� MicroLIB ���� main() ��������Ҫ��ڲ��� */
#if __IS_COMPILER_ARM_COMPILER_6__
    #ifndef __MICROLIB
    __asm(".global __ARM_use_no_argv\n\t");
    #endif
#endif

/* �ر� semihosting */
#if __IS_COMPILER_ARM_COMPILER_6__
__asm(".global __use_no_semihosting");
    
/* AC6 ����Ϊ�ر� semihosting ȱ�������������Ҫ���� */
void _sys_exit(int ret)
{
    (void)ret;
    while(1) {}
}
#elif __IS_COMPILER_ARM_COMPILER_5__
#pragma import(__use_no_semihosting)
#endif

/* AC5 �� AC6 ������Ϊ�ر� semihosting ȱ�������������Ҫ���� */
#if __IS_COMPILER_ARM_COMPILER__
void _ttywrch(int ch)
{
    (void)ch;
}
#endif

/* ��ʹ�� AC6 ���� MicroLIB ʱ������ʹ�� assert() ��������Ҫ�Լ�ʵ�� __aeabi_assert() */
#if __IS_COMPILER_ARM_COMPILER_6__ && defined(__MICROLIB)
void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
{
    (void)chCond;
    (void)chLine;
    (void)wErrCode;
    
    while(1) {
        __NOP();
    }
}
#endif