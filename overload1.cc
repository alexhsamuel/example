#include <iostream>

//------------------------------------------------------------------------------

namespace NA {

namespace Type {

template<class C>
class A
{
public:

  A(C const c) : c_(c) {}
  C get() const { return c_; }

private:

  C const c_;

};


template<class C>
inline C
add(
  C const c0,
  C const c1)
{
  std::cerr << "NA::add()\n";
  return C(c0.get() + c1.get());
}


template<class C> 
inline C operator+(C const c0, C const c1) { return add(c0, c1); }
 
}  // namespace Type

using Type::A;

}  // namespace NA

//------------------------------------------------------------------------------

namespace NB {

namespace Type {

template<class C>
class B
{
public:

  B(C const c) : c_(c) {}
  C get() const { return c_; }

private:

  C const c_;

};


template<class C>
inline C
add(
  C const c0,
  C const c1)
{
  std::cerr << "NB::add()\n";
  return C(c0.get() + c1.get());
}


template<class C> inline C operator+(C const c0, C const c1) 
  { return add(c0, c1); }

}  // namespace Type

using Type::B;

}  // namespace NB

//------------------------------------------------------------------------------

int
main()
{
  using namespace NA;
  using namespace NB;

  auto a0 = A<int>(10);
  auto a1 = A<int>(11);

  auto b0 = B<long>(142l);
  auto b1 = B<long>(150l);

  // auto a2 = add(a0, a1);
  auto a2 = a0 + a1;
  std::cout << "a2.get() -> " << a2.get() << "\n";

  // auto b2 = add(b0, b1);
  auto b2 = b0 + b1;
  std::cout << "b2.get() -> " << b2.get() << "\n";

  return 0;
}

