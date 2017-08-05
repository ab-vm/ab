#include <omr/core/Atomic.hpp>

namespace omr {

    enum class MemoryOrder {
        weak, strong;
    };

    template <typename T>
    class Atomic {
    public:
        Atomic();
        ~Atomic();

        template <MemoryOrder memoryOrder = MemoryOrder::strong>
        bool compareAndSwap(const T& x);

    };

    template <typename T>
    bool Atomic<T>::compareAndSwap<MemoryOrder::weak>() {

    };

    template <typename T>

    namespace Atomic {

    template <typename T>
    struct StaticAssertAtomics {

    };

    template <typename T>
    {
        using Assertions = StaticAssertAtomics<T>;

    };

    }
    atomicCompareAndSwap {

    }
};


namespace omr {
    namespace core {

        enum class Notify {
            one, all
        };

        enum class Expect {
            urgent, delay
        };

        enum class WaitBehaviour {
            spin, sleep, threeTier, normal
        };

        enum class MemoryOrder {

        };

        /// An Atomic<T> with predicate polling.
        /// Implements backoff, so you don't have to.
        template <typename T, enum >
        class Synchronic {
        public:
            Synchronic();
            ~Synchronic();

            notify(std::atomic<T>& atom, T val) noexcept;
            expect(std::atomic<T> atom, T val);

        private:
            Atomic<T> value_;
        };

        class Futex {

        };

        class Mutex {
            while(expect )
        };

        template <typename T>
    }
}
