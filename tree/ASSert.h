#ifndef ASSERT_H
#define ASSERT_H


#ifndef NDEBUG

#define assert(cond)                                                                                                    \
    do {                                                                                                                \
        if (!(cond))                                                                                                    \
        {                                                                                                               \
            printf("ASSERT: %s: %d:" #cond, __FUNCTION__, __LINE__);                                                    \
            exit(1);                                                                                                    \
        }                                                                                                               \
    } while(0)
#else
#define assert(cond)      do {} while(0)
#endif


#endif //ASSERT_H
