#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER
#include <future>

class ControlBlockBase {
public:
    ControlBlockBase() : refCountValue(0) {} // TODO: implement the default constructor.

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase() {} // TODO: implement the destructor.

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        // TODO: increment refcount by 1 and return result.
        refCountValue++;
        return refCountValue;
    }

    long decrement()
    {
        // TODO: decrement refcount by 1 and return result.
        refCountValue--;
        return refCountValue;
    }

    long refCount() const
    {
        // TODO: just return the refcount.
        return refCountValue;
    }

private:
    // TODO: add field(s) which both control block types need to have
    long refCountValue;
};

template <typename T>
class ControlBlock : public ControlBlockBase {
public:
    ControlBlock(T *P = nullptr) : mPtr(P) {}
    ~ControlBlock() {
        if (mPtr) {
            delete mPtr;
        }
    }
    void* managedAddress() override {
        return mPtr;
    }

private:
    T* mPtr;
};

template <typename T>
class SharedPtr {
public:

    //ctor/dtor
    SharedPtr(T *Sp = nullptr) : sPtr{Sp} {
        if (sPtr) {
            cbPtr = new ControlBlock<T>(sPtr);
            cbPtr->increment();
        }else {
            cbPtr = nullptr;
        }
    }
    ~SharedPtr() {
        if (sPtr) {
            cbPtr->decrement();
            if (cbPtr->refCount() == 0) {
                delete cbPtr;
            }
        }
    }

    //Copy ctor
    SharedPtr (const SharedPtr& other) : sPtr (other.sPtr), cbPtr (other.cbPtr) {
        if (cbPtr) {
            cbPtr->increment();
        }
    }

    //Move ctor
    SharedPtr (SharedPtr&& other) : sPtr (other.sPtr) {
        other.sPtr = nullptr;
    }

    //Copy Assignment
    SharedPtr& operator=(const SharedPtr& other)
    {
         SharedPtr otherClone(other);

         std::swap (sPtr, otherClone.sPtr);
        std::swap (cbPtr, otherClone.cbPtr);
         return *this;
     }

    //Move Assignment
    SharedPtr& operator=(SharedPtr&& other) {
        std::swap (sPtr, other.sPtr);
        return *this;
    }

private:
    T *sPtr;
    ControlBlockBase *cbPtr;

};

#endif
