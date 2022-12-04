/**
 * 此文件用于关闭 semihosting
 */
#include "common.h"

/* 告诉编译器若没有使用 MicroLIB ，则 main() 函数不需要入口参数 */
#if __IS_COMPILER_ARM_COMPILER_6__
    #ifndef __MICROLIB
    __asm(".global __ARM_use_no_argv\n\t");
    #endif
#endif

/* 关闭 semihosting */
#if __IS_COMPILER_ARM_COMPILER_6__
__asm(".global __use_no_semihosting");
    
/* AC6 会因为关闭 semihosting 缺这个函数，所以要补上 */
void _sys_exit(int ret)
{
    (void)ret;
    while(1) {}
}
#elif __IS_COMPILER_ARM_COMPILER_5__
#pragma import(__use_no_semihosting)
#endif

/* AC5 和 AC6 都会因为关闭 semihosting 缺这个函数，所以要补上 */
#if __IS_COMPILER_ARM_COMPILER__
void _ttywrch(int ch)
{
    (void)ch;
}
#endif

/* 当使用 AC6 开启 MicroLIB 时，若有使用 assert() 的需求，需要自己实现 __aeabi_assert() */
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