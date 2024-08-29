#include <iostream>

template<class t, size_t n> struct rImpl
{
    template<class ...Args> rImpl(Args&&... args)
    {
        static_assert(sizeof(t) <= n, "too small");
        new (this->buf) t{std::forward<t>(args)...};
    }
    
    operator t& () noexcept
    {
        return *reinterpret_cast<t*>(this->buf);
    }
    
    operator const t& () const noexcept
    {
        return *reinterpret_cast<const t*>(this->buf);
    }
    
    char buf[n];
};

struct some;

using rImpl_t = rImpl<some, sizeof(int)>;

void example(const int v);

int main()
{
    example(33);
}

struct some
{
    some(int v) noexcept
        : v(v)
    {}
    int v;
};

void example(const some& obj)
{
    std::cout << obj.v << '\n';
}

void example(const int v)
{
    const rImpl_t some{v};
    example(some);
}
