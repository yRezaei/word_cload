#include <functional>
#include <string>

namespace helper
{
    using ErrorCallback = std::function<void(const std::string&)>;
}