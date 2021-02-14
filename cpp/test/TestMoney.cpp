#include "gtest/gtest.h"
#include <memory>
#include "TestMoney.h"

std::shared_ptr<const Money> Money::dollar(int amount) {
    return std::make_shared<const Dollar>(amount);
}
std::shared_ptr<const Money> Money::franc(int amount) {
    return std::make_shared<const Franc>(amount);
}



//TODO:  
//  if Frank : Dollar   = 2:1 ;  $5 + F10 = $10
//OK---> $5 * 2 = $10 
//OK -->  $25 * 100 = $2500
//OK --> Equal
//OK--  use money to unify factory
//OK -->  franc != dollar
//OK --> currency
//OK -->  Dollar 不是只读，会有副作用
//OK --> var five  in TC is not good
//  Dollar 可以是小数
//OK -->Franc 5 * 2 = Franc 10
//  franc & dollar duplication
TEST(MainTest, use){

    { //equals
        ASSERT_TRUE(Money::dollar(5)->equals(*Money::dollar(5)));
        ASSERT_FALSE(Money::dollar(5)->equals(*Money::dollar(6)));

        ASSERT_FALSE(Money::dollar(5)->equals(*Money::franc(5)));
    }

    { //times
        auto five = Money::dollar(5);
        ASSERT_TRUE(Money::dollar(10)->equals(*five->times(2)));
        ASSERT_TRUE(Money::dollar(15)->equals(*five->times(3)));
    }

    { //get currency 
        ASSERT_TRUE(Money::franc(5)->getCurrency() == "CHF");
        ASSERT_TRUE(Money::dollar(5)->getCurrency() == "USD");
    }

}

