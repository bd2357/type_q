/**
 * @file test_type_q.c
 * @author Bryce Deary 
 * @brief 
 * @date 2025-09-25
 * 
 * @copyright bdPrime (2025)
 * 
 * @details quick test suite run the same data in the three different ways
 *   of using queues.
 */

#include "unity.h"

#include "type_q.h"

typedef struct test_t
{
    int i1;
    char c1;
} test_t;

DECLARE_TYPE_Q(bob, test_t, 5);
DEFINE_TYPE_Q(bob, test_t, 5);

MAKE_GEN_Q(ted, test_t, 5);

void setUp(void)
{
    bob.reset();
    gen_q_clear(&ted);
}

void tearDown(void)
{
}


void test_type_q_static(void)
{
    // empty
    TEST_ASSERT_EQUAL(0, bob.q_full());
    TEST_ASSERT_NOT_EQUAL(0, bob.q_empty());

    // write one read one
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=9, .i1 = 10}));
    test_t obj;
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(9, obj.c1);
    TEST_ASSERT_EQUAL(10, obj.i1);

    // write to full
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=9, .i1 = 10}));
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=8, .i1 = 11}));
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=7, .i1 = 12}));
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=6, .i1 = 13}));
    TEST_ASSERT_EQUAL(0, bob.q_full());
    TEST_ASSERT_EQUAL(0, bob.in_q(&(test_t){.c1=5, .i1 = 14}));
    TEST_ASSERT_NOT_EQUAL(0, bob.q_full());
    // another write should fail
    TEST_ASSERT_NOT_EQUAL(0, bob.in_q(&(test_t){.c1=4, .i1 = 15}));
    
    // read till empty
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(7, obj.c1);
    TEST_ASSERT_EQUAL(12, obj.i1);    
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(0, bob.q_empty());
    TEST_ASSERT_EQUAL(0, bob.out_q(&obj));
    TEST_ASSERT_EQUAL(5, obj.c1);
    TEST_ASSERT_EQUAL(14, obj.i1);    
    TEST_ASSERT_NOT_EQUAL(0, bob.q_empty());
    // try to read past empty
    TEST_ASSERT_NOT_EQUAL(0, bob.out_q(&obj));

}


void test_gen_q_static(void)
{
    TEST_ASSERT_EQUAL(0, is_gen_q_full(&ted));
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_empty(&ted));

    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=9, .i1 = 10}));
    test_t obj;
    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(9, obj.c1);
    TEST_ASSERT_EQUAL(10, obj.i1);

    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=9, .i1 = 10}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=8, .i1 = 11}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=7, .i1 = 12}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=6, .i1 = 13}) );
    TEST_ASSERT_EQUAL(0, is_gen_q_full(&ted));
    TEST_ASSERT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=5, .i1 = 14}));
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_full(&ted));
    TEST_ASSERT_NOT_EQUAL(0, gen_q_in(&ted, &(test_t){.c1=4, .i1 = 15}));


    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(7, obj.c1);
    TEST_ASSERT_EQUAL(12, obj.i1);    
    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(0, is_gen_q_empty(&ted));
    TEST_ASSERT_EQUAL(0, gen_q_out(&ted, &obj));
    TEST_ASSERT_EQUAL(5, obj.c1);
    TEST_ASSERT_EQUAL(14, obj.i1);    
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_empty(&ted));
    TEST_ASSERT_NOT_EQUAL(0, gen_q_out(&ted, &obj));

}


void test_gen_q_dynamic(void)
{

    test_t some_storage[6];

    gen_q_t fred = bind_gen_q(some_storage, sizeof some_storage, sizeof(test_t));


    TEST_ASSERT_EQUAL(0, is_gen_q_full(&fred));
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_empty(&fred));

    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=9, .i1 = 10}));
    test_t obj;
    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(9, obj.c1);
    TEST_ASSERT_EQUAL(10, obj.i1);

    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=9, .i1 = 10}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=8, .i1 = 11}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=7, .i1 = 12}) );
    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=6, .i1 = 13}) );
    TEST_ASSERT_EQUAL(0, is_gen_q_full(&fred));
    TEST_ASSERT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=5, .i1 = 14}));
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_full(&fred));
    TEST_ASSERT_NOT_EQUAL(0, gen_q_in(&fred, &(test_t){.c1=4, .i1 = 15}));


    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(7, obj.c1);
    TEST_ASSERT_EQUAL(12, obj.i1);    
    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(0, is_gen_q_empty(&fred));
    TEST_ASSERT_EQUAL(0, gen_q_out(&fred, &obj));
    TEST_ASSERT_EQUAL(5, obj.c1);
    TEST_ASSERT_EQUAL(14, obj.i1);    
    TEST_ASSERT_NOT_EQUAL(0, is_gen_q_empty(&fred));
    TEST_ASSERT_NOT_EQUAL(0, gen_q_out(&fred, &obj));

    TEST_ASSERT_EQUAL(sizeof(test_t), gen_q_size(&fred));
    TEST_ASSERT_EQUAL(5, gen_q_len(&fred));


}


