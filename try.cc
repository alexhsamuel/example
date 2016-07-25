// Inspired by https://github.com/oktal/result/blob/master/result.h

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

//------------------------------------------------------------------------------

template<class R, class E> class Result;

namespace impl {

template<class R>
class Ok
{
public:

  Ok(R&& res) : res_(std::move(res)) {}
  Ok(R const& res) : res_(res) {}

  R res_;

};


template<class E>
class Err
{
public:

  Err(E&& err) : err_(std::move(err)) {}
  Err(E const& err) : err_(err) {}

  E err_;

};


}  // namespace impl

template<class R>
impl::Ok<typename std::decay<R>::type>
Ok(
  R&& res)
{
  return std::forward<R>(res);
}


template<class E>
impl::Err<typename std::decay<E>::type>
Err(
  E&& err)
{
  return std::forward<E>(err);
}


template<class R, class E>
class Result
{
public:

  Result(impl::Ok<R>&& res) : res_(std::move(res.res_)), err_(), ok_(true) {}
  Result(impl::Err<E>&& err) : res_(), err_(std::move(err.err_)), ok_(false) {}

  bool ok() const { return ok_; }

  R const&
  res()
    const
  {
    if (ok_)
      return res_;
    else
      throw std::runtime_error(err_.to_string());
  }
    
  E const& 
  err()
    const
  {
    if (ok_)
      throw std::runtime_error("no error");  // FIXME: Another type.
    else
      return err_;
  }

private:

  R res_;
  E err_;
  bool ok_;

};


//------------------------------------------------------------------------------

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class FileErr
{
public:

  FileErr() = default;
  FileErr(FileErr&&) = default;
  FileErr(FileErr const&) = default;

  FileErr(
    std::string const& filename, 
    std::string const& description)
  : filename_(filename),
    description_(description)
  {
  }
  
  std::string 
  to_string() 
    const
  {
    return filename_ + ": " + description_;
  }

private:

  std::string filename_;
  std::string description_;

};


using Fd = int;

Result<Fd, FileErr>
open_read(
  std::string const& filename)
{
  Fd const fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1)
    return Err(FileErr(filename, "can't open"));  // FIXME: Check errno.
  else
    return Ok(fd);
}



Result<std::string, FileErr>
read_file(
  std::string const& filename)
{
  auto const fd_res = open_read(filename);
  if (!fd_res.ok())
    return Err(fd_res.err());
  auto const fd = fd_res.res();

  char buf[1024];
  int const num_read = read(fd, buf, sizeof(buf) - 1);
  buf[num_read] = 0;
  return Ok(std::string(buf));
}


//------------------------------------------------------------------------------

int
main(
  int const argc,
  char const* const* const argv)
{
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " FILENAME\n";
    return 2;
  }
  std::string const filename = argv[1];

  auto const content_res = read_file(filename);
  if (!content_res.ok()) {
    std::cerr << content_res.err().to_string() << "\n";
    return 1;
  }
  auto const content = content_res.res();

  std::cout << content << "\n";
  return 0;
}


