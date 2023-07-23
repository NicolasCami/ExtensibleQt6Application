#pragma once

namespace app { class IApp; }

namespace py {

class Instance
{
public:
    Instance(::app::IApp* app);
    ~Instance();

private:
    ::app::IApp* m_appInterface{nullptr};
};

}
