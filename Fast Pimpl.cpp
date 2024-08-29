#include <print>
#include <utility>

template <typename T, std::size_t Size, std::size_t Align>
struct FastPimpl {
  template <typename... Ts>
  FastPimpl(Ts&&... args) {
    new(this->buffer) T{std::forward<Ts>(args)...};
  };
  template <typename Self>
  auto Get(this Self&& self) {
    return std::forward_like<Self>(
        *std::launder(reinterpret_cast<std::remove_reference_t<decltype(std::forward_like<Self>(std::declval<T&&>()))>*>(self.buffer)));
  };

 private:
  alignas(Align) std::byte buffer[Size];
};

struct Some {
  struct SomeImpl;
  using Pimpl = FastPimpl<SomeImpl, sizeof(int), alignof(int)>;
  auto Method() const -> int;

 private:
  Pimpl obj;
};

auto main() -> int {
  Some obj{};
  std::println("{}", obj.Method());
};

// For cpp

struct Some::SomeImpl {
  auto Method() const -> int {
    return this->data;
  };
  int data;
};

auto Some::Method() const -> int {
  return this->obj.Get().Method();
};
