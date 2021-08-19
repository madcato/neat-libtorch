#ifndef SHARED_HELPER_HPP
#define SHARED_HELPER_HPP

#include <memory>

// This class is based on PyTorch ModuleHolder implementation

/// A `ObjectHolder` is essentially a wrapper around `std::shared_ptr<M>` where
/// `M` is any class, with convenient constructors defined for
/// the kind of constructions we want to allow for our modules.
template <typename Contained>
class ObjectHolder {
 protected:
    std::shared_ptr<Contained> impl_;

 public:
    /// The module pointer this class wraps.
    /// NOTE: Must be placed at the top of the class so that we can use it with
    /// trailing return types below.
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes)
    using ConteinedType = Contained;

    /// Default constructs the contained module if if has a default constructor,
    /// else produces a static error.
    ///
    /// NOTE: This uses the behavior of template
    /// classes in C++ that constructors (or any methods) are only compiled when
    /// actually used.
    ObjectHolder(): impl_(default_construct()) {
        static_assert(
            std::is_default_constructible<Contained>::value,
            "You are trying to default construct a module which has "
            "no default constructor. Use = nullptr to give it the empty state "
            "(e.g. `Linear linear = nullptr;` instead of `Linear linear;`).");
    }

    ObjectHolder(Contained* obj): impl_(obj) {
    }

    /// Returns true if the `ModuleHolder` contains a module, or false if it is
    /// `nullptr`.
    explicit operator bool() const noexcept {
        return !is_empty();
    }

     /// Forwards to the contained module.
    Contained* operator->() {
        return get();
    }

    /// Forwards to the contained module.
    const Contained* operator->() const {
        return get();
    }

    /// Returns a reference to the contained module.
    Contained& operator*() {
        return *get();
    }

    /// Returns a const reference to the contained module.
    const Contained& operator*() const {
        return *get();
    }

    /// Returns a shared pointer to the underlying module.
    const std::shared_ptr<Contained>& ptr() const {
        assert(!is_empty());  // "Accessing empty ModuleHolder"
        return impl_;
    }

    /// Returns a pointer to the underlying module.
    Contained* get() {
        assert(!is_empty());  // "Accessing empty ModuleHolder"
        return impl_.get();
    }

    /// Returns a const pointer to the underlying module.
    const Contained* get() const {
        assert(!is_empty());  // "Accessing empty ModuleHolder"
        return impl_.get();
    }

    /// Returns true if the `ModuleHolder` does not contain a module.
    bool is_empty() const noexcept {
        return impl_ == nullptr;
    }

    bool operator==(const ObjectHolder& other) const {
        return *impl_ == *other;
    }

    bool operator<(const ObjectHolder& other) const {
        return *impl_ < *other;
    }

friend class ObjectHolder;

private:
    /// In C++17, the two methods below could be written as the following:
    /// if constexpr (std::is_default_constructible_v<Contained>) {
    ///   return std::make_shared<Contained>();
    /// } else {
    ///   return nullptr;
    /// }
    /// In C++11, we use SFINAE instead of `if constexpr`.

    // template <
    //     typename T = Contained,
    //     typename = torch::enable_if_t<std::is_default_constructible<T>::value>>
    // std::shared_ptr<Contained> default_construct() {
    //     return std::make_shared<Contained>();
    // }

    Contained* default_construct() {
        return nullptr;
    }
 };

#define SHARED_IMPL(Name, ImplType)                 \
    class Name: public ObjectHolder<ImplType> {     \
     public:                                        \
        using ObjectHolder<ImplType>::ObjectHolder; \
        using Impl = ImplType;                      \
    }

#define SHARED_HELPER(Name) SHARED_IMPL(Name, Name##Impl)

#endif  // SHARED_HELPER_HPP