#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <linkedList.h>

START_TEST(list_insert_and_remove){
  struct list l;

  ll_initialize(&l);

  int ints[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  struct list_node *mk;
  for(int i = 0; i < 10; i++){
    mk = ll_insert_val(&l, (void*)&ints[i]);
    ck_assert_ptr_eq(mk, ll_search_node(&l, (void*)&ints[i], sizeof(int)));
  }
  



}
END_TEST


Suite * list_suite(void){

  Suite *s;
  TCase *tc_core;

  s = suite_create("linked_list");


  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, list_insert_and_remove);
  suite_add_tcase(s, tc_core);

  return s;

}

int main(){

  int number_failed;
  Suite *s;
  SRunner *sr;
  
  s = list_suite();
  sr = srunner_create(s);
  
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
  
}
