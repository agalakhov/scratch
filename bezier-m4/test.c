#include <stdio.h>
#include <stdint.h>

typedef uint32_t vec2s16_t;

static inline vec2s16_t pkhtb(int32_t x, int32_t y)
{
    vec2s16_t v;
    asm("pkhtb %0, %1, %2, asr #16"
        : "=r"(v)
        : "r"(x), "r"(y)
        :
    );
    return v;
}

static inline int32_t smuad(vec2s16_t a, vec2s16_t b)
{
    int32_t x;
    asm("smuad %0, %1, %2"
        : "=r"(x)
        : "r"(a), "r"(b)
        :
    );
    return x;
}

int main()
{
    vec2s16_t v = pkhtb(7<<16, 2<<16);
    printf("0x%08x\n", v);

    int32_t x = smuad(0x00010002, 0x00020001);
    printf("%i\n", x);


    return 0;
}
