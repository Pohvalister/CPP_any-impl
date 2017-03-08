#ifndef MY_ANY_H
#define MY_ANY_H

#include <typeinfo>

struct my_any {
    //creation

    template<typename T1>
    my_any(T1&& value) {
        if (sizeof(std::decay_t<T>)<=SIZE)
        new (&storageH) T1(ttemp_holder <T1> tmpBH = temp_holder<T1>(value);
        else
            prtBH = new temp_holder<T1>(tmpBH);
    }

    ~my_any() {
        if (!tooSmall)
            delete ptrBH;
    }
    //\creation

    template<typename T2>
    T2 cast() {
        base_holder *tmpPtr;
        if (tooSmall)
            tmpPtr = &smallBH;
        else
            tmpPtr = prtBH;

        if (prtBH->type() != typeid(T2))
            throw "wrong type cast";
        return static_cast<U>(tmpPtr->value);
    }

private :
    //holdingStruct
    struct any_holder {
        virtual std::type_info type() = 0;

        virtual ~any_holder() { };
    };

    template<typename T>
    struct temp_holder : any_holder {
        temp_holder(const T &val) : value(val) {};
        temp_holder(T&& val): value(std::move(val)){};

        temp_holder(const temp_holder<T> &valBH) : value(valBH.value) { }

        std::type_info type() const {
            return typeid(value);
        }

        T value;
    };
    //\holdingStruct

    //placement
    static size_t SIZE=128;
    typename std::aligned_storage<SIZE, SIZE>::type storageH;
    any_holder* ptrH;
    //\placement
};

#endif