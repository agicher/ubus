#include <stdio.h>

#define STR "right"

// void str_cmp(char *str)
// {
//     printf("STR_addr %p\n",&STR);
//     if (STR == str)
//     {
//         printf("str_addr %p\n",str);
//     }
    
// }
int main()
{
    // char *str = "righ";
    // printf("main_add %p\n",str);
    // str_cmp(str);

    int newcount = 3;
    printf("%d\n", newcount <<= 1);
    return 0;
}