/**
 * @file type_q.c
 * @author Bryce Deary 
 * @brief 
 * @date 2025-09-25
 * 
 * @copyright bdPrime (2025)
 * 
 * @details memory resident functions
 */
#include "type_q.h"

#ifdef TEST
#define __no_inline_not_in_flash_func(func) func
#endif

 
int __no_inline_not_in_flash_func(is_gen_q_full)(gen_q_t *gen) 
{
    uint8_t const *prev = gen->out - gen->size;
    if(prev < gen->buf) prev = gen->end - gen->size; 
    return gen->in == prev;
}

int __no_inline_not_in_flash_func(gen_q_in)(gen_q_t *gen, void const *in)
{   
    uint8_t *next = gen->in + gen->size;
    if(next == gen->end) next = gen->buf; 
    if(next != gen->out) { 
        memcpy( gen->in, in, gen->size);
        gen->in = next; 
        return 0; } 
    return -1; 
}  

/* non zero return if no data */ 
int __no_inline_not_in_flash_func(gen_q_out)(gen_q_t *gen, void *out)
{   if(gen->in == gen->out) return -1;  /*no data*/   
    uint8_t *next = gen->out + gen->size;
    if(next == gen->end) next = gen->buf; 
    memcpy(out, gen->out, gen->size); /* *out = *gen->out++; */ 
    gen->out = next;
    return 0; 
} 


