#include <stdio.h>

#include <sys/socket.h>

// 以 S3C6410 RTC 驱动示例:
// Linux 下的 C 代码风格:
// Documentation/CodingStyle
// scripts/checkpatch.pl 提供了一个检查代码风格的脚本,如果异常会报警告

/* 宏 */
#define PI      (3.1414926)

/* 函数命名 */
void send_data(void);

/* { 的使用*/
struct var_data {
    int len;
    char data[0];
};

int main(int argc, char *argv[])
{
    
    return 0;
}

void send_data(void)
{ // 对于函数 {} 需要另起一行
    // 编码风格:
    int a, b, c, d;
    char suffix;

    if (a == b) {
        a = c;
        d = a;
    }

    // 如果 if/for 后面只有一行,不要加 {}
    for (int i = 0; i < 10; ++i)
        a = c;

    if (a == b)
        a = c;

    /* switch 和 case 对齐 */
    switch (suffix) {
    case 'A':
    case 'a':
        a = 1;
        break;
    case 'B':
    case 'b':
        b = 2;
    default:
        break;
    }
}

/* GNU C 与 ANSI C */
