#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "Tlib_version.h"

#define PRIVATE_TEST 1

#include <Tlocale.h>
#include <Tiostream.h>

TEST(version, test1)
{
      ASSERT_TRUE(version() > 0);
}

namespace tlib
{
      //class PublisherMixinTest : public testing::Test
      //{
      //  public:
      //      void SetUp() {}
      //      void TearDown() {}
      //};

      //TEST_F(PublisherMixinTest, UniqueAddSubscribers1)
      //{
      //      //pbl.subscribe(subscriber_functor()); //  rvalue - добавляем повторно
      //      //ASSERT_TRUE(pbl.subscribers.size() == 7);
      //}


      TEST(Tlocale, create_locale_by_any_name)
      {
            // Проверяем всевозможные форматы задание имени локали
            // Формат имени локали не должен зависеть от платформы

            try
            {
                  tlib::locale("C");
                  tlib::locale("POSIX");
                  tlib::locale("ru_RU");
                  tlib::locale("ru-RU");
                  tlib::locale("Russian_Russia");
                  tlib::locale("Russian-Russia");

                  tlib::locale(".UTF8");
                  tlib::locale("ru_RU.Utf-8");
                  tlib::locale("ru_RU.uTf-8");
                  tlib::locale("ru_RU.uTf8");

                  tlib::locale("ru_RU.utf-8");
                  tlib::locale("ru-RU.utf-8");
                  tlib::locale("Russian_Russia.utf-8");
                  tlib::locale("Russian-Russia.utf-8");
            }
            catch (...)
            {
                  ASSERT_TRUE(false);
            }
            ASSERT_TRUE(true);            
      }

}
