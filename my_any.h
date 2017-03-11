#ifndef MY_ANY_H
#define MY_ANY_H

#include <typeinfo>
#include <memory>
#include "iostream"


enum storage_status{
    isSmall,
    isBig,
    isEmpty
};
struct my_any {

private :
    //holdingStruct
    struct any_holder {
        virtual const std::type_info& type() const = 0;
    };


    template<typename T>
    struct temp_holder :  any_holder {
        temp_holder(const T &val) : valueH(val) {};
        temp_holder(T&& val): valueH(std::move(val)){};

        const std::type_info& type() const {
            return typeid(valueH);
        }
        T valueH;
    };
    //\holdingStruct


    //placement
    static const size_t MAX_SIZE=128;
    typename std::aligned_storage<MAX_SIZE, MAX_SIZE>::type storageH;
    storage_status status;
    void (*copier)(void*,void const*);
    void (*deleter)(void*);
    //\placement

public:
    void* holderPointer(){
        if (status==isSmall){
            return (reinterpret_cast<void*>(&storageH));
        }
        if (status==isBig){
            return *(void**)&storageH;
        }
        return nullptr;
    }
    //creation
    my_any():deleter([](void*){}),copier([](void*,void const*){}){
        status=isEmpty;
    }

    template<typename T1>
    my_any(const T1& value) {
        typedef typename std::remove_cv<const T1>::type nT1;
        if (sizeof(temp_holder<nT1>) <= MAX_SIZE){
            status = isSmall;
            new(&storageH) temp_holder<nT1>(temp_holder<nT1>(value));
            deleter = [](void* currentStorage){((temp_holder<nT1>*)currentStorage)->~temp_holder<nT1>();};//destructor of container
            copier = [](void* curSt,const void* othSt){
                new(curSt) temp_holder<nT1>(temp_holder<nT1>(std::forward<nT1>(((temp_holder<nT1>*)othSt)->valueH)));
            };
        }
        else {
            status = isBig;
            new(&storageH) temp_holder<nT1> *(new temp_holder<nT1>(value));
            deleter = [](void *currentStorage) { delete (*((temp_holder<nT1>**)currentStorage)); };//destructor of link of storage
            copier = [](void * curSt,const void * othSt){
                new (curSt) temp_holder<nT1>* (new temp_holder<nT1>((*((temp_holder<nT1>**)othSt))->valueH));
            };
        }
    }

    my_any (my_any && other):status(other.status),deleter(other.deleter), copier(other.copier){
        other.copier(&storageH,std::move(&other.storageH));
    }
    my_any (const my_any &other):status(other.status),deleter(other.deleter),copier(other.copier){
        other.copier(&storageH,&other.storageH);
    }
    ~my_any() {
        deleter(&storageH);
    }

    //\creation
    my_any& swap(my_any& other) {
        std::swap(storageH, other.storageH);
        std::swap(status,other.status);
        std::swap(deleter,other.deleter);
        std::swap(copier,other.copier);
        return *this;
    }
    template <typename T>
    my_any& operator=(T&& other) {
        my_any(std::move(other)).swap(*this);
        return *this;
    }

    my_any& operator=(my_any&& other){
        other.swap(*this);
        return *this;
    }

    bool empty() const{
        return status==isEmpty;
    }

    void clear(){
        my_any().swap(*this);
    }

    template<typename T3>
    T3 cast() {
        if (status==isSmall) {
            if (reinterpret_cast<any_holder*>(&storageH)->type()!= typeid(T3))
                throw "wrong type cast";
            return ((temp_holder<T3>*)(&storageH))->valueH;
        }
        if (status==isBig){
            if ((*reinterpret_cast<any_holder**>(&storageH))->type()!= typeid(T3))
                throw "wrong type cast";
            return (*(temp_holder<T3>**)(&storageH))->valueH;
        }
        return NULL;
    }

    //external things
    template<typename T> friend T* any_cast(my_any*);
    template<typename T> friend const T* any_cast(const my_any*);
    template<typename T> friend T any_cast(my_any& oper);
    template<typename T> friend T any_cast(const my_any& oper);
    template<typename T> friend T any_cast(my_any&& oper);
    //\external things

};

void swap (my_any& a, my_any& b){
    a.swap(b);
}

template <typename T>
T* any_cast(my_any* oper){
        my_any::any_holder * tmpPointer= reinterpret_cast<my_any::any_holder*>(oper->holderPointer());
    if (tmpPointer->type()== typeid(T))
        return &(reinterpret_cast<my_any::temp_holder<T>*>(tmpPointer))->valueH;
    return nullptr;

}
template <typename T>
const T* any_cast(const my_any* oper){
    return any_cast<T>(const_cast<my_any*>(oper));
}
template <typename T>
T any_cast(my_any& oper){
    typedef typename std::remove_reference<T>::type tmpT;
    return *any_cast<tmpT>(&oper);
}
template <typename T>
T any_cast(const my_any& oper){
    return any_cast<const typename std ::remove_reference<T>::type &>(const_cast<my_any&>(oper));
}
template <typename T>
T any_cast(my_any&& oper){
    return any_cast<T>(oper);
}

#endif
/*
typedef typename std::remove_reference<T>::type non;
non* res = any_cast<non>(&oper);
return res;
 */