/**
*
* Program: testCalc.c - Calculator Test Implementation
* Autor: Diego Pacheco
* Since: 02/MAY/2018
* Version: 1.0
*
**/
#include <check.h>
#include "../src/calc.h"
#include <stdlib.h>
#include <stdio.h>

START_TEST(test_new_calc) {
  extern Calc *newCalc();

  Calc *c = newCalc();
  ck_assert_uint_eq(c->sum(1,1), 2);
  printf(" [PASS] Test: 1 + 1 == 2 \n");

  ck_assert_uint_eq(c->sub(1,1), 0);
  printf(" [PASS] Test: 1 - 1 == 0 \n");

  ck_assert_uint_eq(c->mul(5,5), 25);
  printf(" [PASS] Test: 5 * 5 == 25 \n");

  ck_assert_uint_eq(c->divi(5,5), 1);
  printf(" [PASS] Test: 5 / 5 == 1 \n");

  destroyCalc(c);
} END_TEST

Suite *calc_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("CalcTest");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_new_calc);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  Suite *s = calc_suite();
  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  int no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
