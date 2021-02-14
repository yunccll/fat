
#ifndef  TESTMONEY_H
#define  TESTMONEY_H


class Money {
public:
    static std::shared_ptr<const Money> dollar(int amount) ;
    static std::shared_ptr<const Money> franc(int amount);

    Money(int amount, const std::string & currency)
    : amount(amount)
    , currency(currency)
    {
    }
    virtual ~Money(){
    }

    const int getAmount() const {
        return amount;
    }

    std::shared_ptr<const Money> times(int multiplier) const{
        return std::make_shared<const Money>(amount * multiplier, currency);
    }

    bool equals(const Money & expect) const {
        return (this->amount == expect.amount) 
            && (this->currency == expect.currency);
    }
    const std::string & getCurrency() const {
        return currency;
    }

protected:
    const int amount; 
    const std::string currency;
};

class Dollar : public Money {
public:
    explicit Dollar(int amount)
    : Money(amount, "USD")
    {
    }
};

class Franc : public Money {
public:
    explicit Franc(int amount)
    : Money(amount, "CHF")
    {
    }
};


#endif   /* TESTMONEY_H */
