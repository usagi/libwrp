#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

namespace wrp { 

#if !defined(WRP_PRODUCT_NAME)
  #define WRP_PRODUCT_NAME "UNKNOWN PRODUCT"
#endif

  class log{
  private:
    const decltype(std::chrono::system_clock::now()) start;
    const std::string object_name;
    const void* object_address;
    static int nesting_counter;
  public:
    typedef std::chrono::duration<double> unit;
    std::stringstream buffer;
    explicit log(const std::string object_name_ = "", const void* object_address_ = 0)
      : start(std::chrono::system_clock::now())
      , object_name(object_name_)
      , object_address(object_address_)
    {
      ++nesting_counter;
    }
    ~log(){
      using std::chrono::duration_cast;
      auto end = std::chrono::system_clock::now();
      std::string indent;
      for(auto n = nesting_counter; n; --n)
        indent += "  ";
      std::cout
        << indent << "[" WRP_PRODUCT_NAME "] "
                  << object_name << " " << object_address << "\n"
        << indent << "start: "
                  << duration_cast<unit>(start.time_since_epoch()).count() << "\n"
        << indent << "end  : "
                  << duration_cast<unit>(end.time_since_epoch()).count() << "\n"
        << indent << "dt   : "
                  << duration_cast<unit>((end - start)).count() << "\n"
        ;
      std::string b;
      while(std::getline(buffer, b))
        std::cout << indent << b << "\n";
      std::cout << std::flush;
      --nesting_counter;
    }
    auto operator<<(const bool v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(const char* v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(std::string& v) -> decltype(buffer<<v) { return buffer << v; }
    auto operator<<(int& v) -> decltype(buffer<<v){ return buffer << v; }
  };
  
  int log::nesting_counter = -1;
  
}

