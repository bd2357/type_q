/**
 * @file type_q.h
 * @author Bryce Deary
 * @brief generic queue builders
 * @date 2025-09-22
 * 
 * @copyright bdprime (2025)
 * 
 * @details macros to build a queue of an object type and a 
 *  general purpose queue type with operators.
 * 
 *  Different ways of solving the same problem of makeing queues for passing around
 *  objects. Use the gen_q module for dealing with many queues of different 
 *  sizes and lengths in one application. 
 * 
 *  Use TYPE_Q macros for making bespoke queues where both sides have access to
 *  the declaration
 * 
 * 
 */
#ifndef TYPE_Q_H
#define TYPE_Q_H

#include <string.h>
#include <stdint.h>

typedef struct gen_q_t 
{
    uint8_t * const buf;        // head of the buffer store
    uint8_t const * const end;  // address of first byte beyond buffer store
    uint8_t *in;                // next in point in queue
    uint8_t *out;               // next out point of queue
    uint32_t const size;        // the size of the object in the queue
    uint32_t const len;         // the usable length (one less than allocated storage)
} gen_q_t;

/**
 * @brief sets the queue to empty
 * 
 * @param gen[in/out] pointer to the queue object
 */
static inline void gen_q_clear(gen_q_t *gen) { gen->out = gen->in;}

/**
 * @brief Get the usable length of the queue
 * 
 * @param gen[in] pointer to the queue object
 * @return uint32_t the Max number of object the queue can hold.
 */
static inline uint32_t gen_q_len(gen_q_t *gen) { return gen->len; }

/**
 * @brief gets the size in bytes of the managed object type
 * 
 * @param gen[in] pointer to the queue object
 * @return uint32_t the sizeof(object type)
 */
static inline uint32_t gen_q_size(gen_q_t *gen) { return gen->size; }

/**
 * @brief return non zero when empty
 * 
 * @param gen[in] 
 * @return int 0 if queue has data, 1 if queue is empty
 */
static inline int is_gen_q_empty(gen_q_t *gen) {return gen->in == gen->out;}

/**
 * @brief Extract the oldest item in the queue
 * 
 * @param gen[in/out] pointer to the queue object
 * @param out[out] a pointer to a destination memory location overwritten by the function
 * @return int non zero on error, no data in the queue 
 */
int gen_q_out(gen_q_t *gen, void *out);

/**
 * @brief Add the newest item into the queue
 * 
 * @param gen[in/out] pointer to the queue object
 * @param in[in/out] pointer in memory of the object to copy
 * @return int  non zero if no space is available, no changes will be made.
 */
int gen_q_in(gen_q_t *gen, void const *in);

/**
 * @brief test if the queue is full
 * 
 * @param gen[in/out] pointer to the queue object
 * @return int non zero if queue is full
 */
int is_gen_q_full(gen_q_t *gen);

// run time constructor of a general queue

/**
 * @brief Run time constructor to bind a buffer of objects to a queue object
 * 
 * @param store[in] pointer to storage in memory
 * @param store_size[in] the length of the store in bytes
 * @param obj_size[in] the sizeof the managed type
 * @return gen_q_t a gen_q_t object (not pointer), initialized for the provided
 *                   storage and object definition
 */
static inline gen_q_t bind_gen_q(void * const store, uint32_t store_size, uint32_t obj_size)
{
    return (gen_q_t){
        .buf = store,
        .len = (store_size / obj_size) - 1, // usable len is one less
        .end = &(((uint8_t*)store)[(store_size / obj_size)*obj_size]),
        .size = obj_size,
        .in = store,
        .out = store,
    };
}

/**
 * @brief compile time constructor macro for general queue
 *  creates a gen_q_t object of the given name bound to an instantiated
 *  storage block created from the type and qlen information.
 */
#define MAKE_GEN_Q(name, type, qlen) \
/* make storeage one bigger to get full length */ \
static type name ## Store[ qlen+1 ]; \
gen_q_t name = (gen_q_t) { \
    .size = sizeof(type), \
    .len = qlen, \
    .buf = (uint8_t*) name ## Store, \
    .in = (uint8_t*) name ## Store, \
    .out = (uint8_t*) name ## Store,\
    .end = (uint8_t*) &name ## Store[ qlen+1 ], \
}


/**
 * @brief macro for declaring a named queue of specified type and length
 * 
 */
#define DECLARE_TYPE_Q(name, type, len) \
typedef struct name ## _q_t {    \
    type * const buf; \
    type const * const end; \
    type *in; \
    type *out; \
    int (*in_q)( type const *obj_in); \
    int (*out_q)( type *obj_out); \
    int (*q_empty)(void); \
    int (*q_full)(void); \
    void (*reset)(void); \
} name ## _q_t; \
extern name ## _q_t  name

/* The allocated storage is one object larger than length */

/**
 * @brief macro for defining a named queue of specified type and length
 *  including instantiating the underlying storage and initializing the
 *  named queue object.
 */
#define DEFINE_TYPE_Q(name, type, len) \
type name ## Store[len+1]; \
/* non zero return if no store */ \
int name ## _in(type const *in) \
{   type *next =  name.in + 1; \
    if(next == name.end) next = name.buf; \
    if(next != name.out) { \
        /* memcpy instead of assignment for miss aligned src */ \
        memcpy( name.in, in, sizeof(type)); \
        name.in = next; \
        return 0; } \
    return -1; \
}  \
/* non zero return if no data */ \
int name ## _out(type *out) \
{   if(name.in == name.out) return -1;  /*no data*/ \
    memcpy(out, name.out++, sizeof *out); /* *out = *name.out++; */ \
    if(name.out == name.end) name.out = name.buf; \
    return 0; \
} \
int name ## _q_empty(void){return name.in == name.out;} \
int name ## _q_full(void){type const *prev = name.out -1; \
    if(prev < name.buf) prev = name.end - 1; \
    return name.in == prev;} \
void name ## _q_reset(void) { \
    name.in = name ## Store, \
    name.out = name ## Store;} \
name ## _q_t  name = { \
    .buf = name ## Store, \
    .end = & name ## Store[len+1], \
    .in = name ## Store, \
    .out = name ## Store, \
    .in_q = name ## _in, \
    .out_q = name ## _out, \
    .q_empty = name ## _q_empty, \
    .q_full = name ## _q_full, \
    .reset = name ## _q_reset, \
}

#endif // TYPE_Q_H
