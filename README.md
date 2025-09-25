# type_q

generic C object queues

## Named Macro method

MAKE_TYPE_Q is a set of macros to instantiate a queue os a given type and length, 
where the named queue object includes methods for using the queue.

## General memory method

The MAKE_GEN_Q macro or the bind_gen_q function create a generic q object that
is initialized to manage an object queue of a fixed object size and length.
The methods are general purpose and methods ae included for extracting the 
size of the managed object type.

