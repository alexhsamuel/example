template<class T>
struct C
{
  C(T const x_) : x(x_) {}
  template<class U> C(C<U> const c) : x(c.x) {}
  template<class U> operator C<U>() { return C<U>(x); }
  T x;
};

template<class T, class U>
inline bool
operator==(
  C<T> c0,
  C<U> c1)
  noexcept
{
  return c0.x == c1.x;
}

bool
foo()
{
  C<int> c0{10};
  C<long> c1{10};
  return c0 == c1;
}

