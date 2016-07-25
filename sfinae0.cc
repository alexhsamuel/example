namespace NA {
  class A { public: void a() {} };
  template<class C> void foo(C const c) { c.a(); }
}

namespace NB {
  class B { public: void b() {} };
  template<class C> void foo(C const c) { c.b(); }
}

int main() { 
  using namespace NA;
  using namespace NB;
  NB::foo(A()); 
  return 0; 
}
