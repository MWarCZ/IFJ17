//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: test/sample.test.c           //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#include "../src/sample.h"

#include "../test_framework/cheat.h"

CHEAT_TEST(Test_FirstTest,
  cheat_assert(1 == 1);
  cheat_assert_not( 2 == 1);
)

CHEAT_TEST(Test_SecondTest,
  cheat_assert( 1 == 1);
)

CHEAT_TEST(Test_ThirdTest,
  cheat_assert_not( 3 == 1);
)

CHEAT_TEST(Test_ForCyclus,
  for(int i = 0; i < 10; i++) {
    cheat_assert(i == i);
  }
)

CHEAT_TEST(Test_GetNumber,
	cheat_assert(GetNumber(1) == 1);
)


// Soubor: test/sample.test.c

