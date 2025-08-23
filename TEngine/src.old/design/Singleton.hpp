#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class Singleton {
   private:
    static T instance;

   public:
    static T& GetInstance() {
        return instance;
    }
};

template <typename T>
T Singleton<T>::instance = T();

#endif