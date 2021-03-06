#include <yaml-cpp/yaml.h>
#include <fstream>
#include "settings.hpp"

namespace YAML {
    template<>
        struct convert<florb::cfg_tileserver> {
            static Node encode(const florb::cfg_tileserver& rhs) {
                Node node;
                node["name"] = rhs.name();
                node["url"] = rhs.url();
                node["zmin"] = rhs.zmin();
                node["zmax"] = rhs.zmax();
                node["parallel"] = rhs.parallel();

                node["type"] = "PNG";
                if      (rhs.type() == florb::image::PNG)
                    node["type"] = "PNG";
                else if (rhs.type() == florb::image::JPG)
                    node["type"] = "JPG";

                return node;
            }

            static bool decode(const Node& node, florb::cfg_tileserver& rhs) 
            {
                if((!node.IsMap()) || (node.size() == 0))
                    return true;
                
                if (node["name"])
                    rhs.name(node["name"].as<std::string>());

                if (node["url"])
                    rhs.url(node["url"].as<std::string>());

                if (node["zmin"])
                    rhs.zmin(node["zmin"].as<int>());
                
                if (node["zmax"])
                    rhs.zmax(node["zmax"].as<int>());
                
                if (node["parallel"])
                    rhs.parallel(node["parallel"].as<int>());

                if (node["type"])
                {
                    rhs.type(florb::image::PNG);
                    std::string imgtype = node["type"].as<std::string>();
                    if      (imgtype.compare("PNG") == 0)
                        rhs.type(florb::image::PNG);
                    else if (imgtype.compare("JPG") == 0)
                        rhs.type(florb::image::JPG);
                }

                return true;
            }
        };

    template<>
        struct convert<florb::cfg_cache> {
            static Node encode(const florb::cfg_cache& rhs) {
                Node node;
                node["location"] = rhs.location();
                return node;
            }

            static bool decode(const Node& node, florb::cfg_cache& rhs) 
            {
                if((!node.IsMap()) || (node.size() == 0))
                    return true;

                if (node["location"])
                    rhs.location(node["location"].as<std::string>());
                
                return true;
            }
        };

    template<>
        struct convert<florb::cfg_units> {
            static Node encode(const florb::cfg_units& rhs) {
                Node node;

                switch (rhs.system_length())
                {
                    case (florb::cfg_units::system::IMPERIAL):
                        node["system_length"] = "imperial";
                        break;
                    case (florb::cfg_units::system::NAUTICAL):
                        node["system_length"] = "nautical";
                        break;
                    default:
                        node["system_length"] = "metric";
                        break;
                }

                return node;
            }

            static bool decode(const Node& node, florb::cfg_units& rhs) 
            {
                if((!node.IsMap()) || (node.size() == 0))
                    return true;

                if (node["system_length"])
                {
                    std::string sm(node["system_length"].as<std::string>());
                    if (sm == "imperial")
                        rhs.system_length(florb::cfg_units::system::IMPERIAL);
                    else if (sm == "nautical")
                        rhs.system_length(florb::cfg_units::system::NAUTICAL);
                    else
                        rhs.system_length(florb::cfg_units::system::METRIC);
                }
                
                return true;
            }
        };

    template<>
        struct convert<florb::cfg_gpsd> {
            static Node encode(const florb::cfg_gpsd& rhs) {
                Node node;
                node["enabled"] = rhs.enabled();
                node["host"] = rhs.host();
                node["port"] = rhs.port();
                return node;
            }

            static bool decode(const Node& node, florb::cfg_gpsd& rhs) 
            {
                if((!node.IsMap()) || (node.size() == 0))
                    return true;

                if (node["enabled"])
                    rhs.enabled(node["enabled"].as<bool>());
                
                if (node["host"])
                    rhs.host(node["host"].as<std::string>());
                
                if (node["port"])
                    rhs.port(node["port"].as<std::string>());
                
                return true;
            }
        };

    template<>
        struct convert<florb::cfg_ui> {
            static Node encode(const florb::cfg_ui& rhs) {
                Node node;
                node["markercolor"] = rhs.markercolor().rgb();
                node["markercolorselected"] = rhs.markercolorselected().rgb();
                node["trackcolor"] = rhs.trackcolor().rgb();
                node["selectioncolor"] = rhs.selectioncolor().rgb();
                node["gpscursorcolor"] = rhs.gpscursorcolor().rgb();
                node["tracklinewidth"] = rhs.tracklinewidth();
                return node;
            }

            static bool decode(const Node& node, florb::cfg_ui& rhs) 
            {
                // Use defaults
                if((!node.IsMap()) || (node.size() == 0))
                {
                    return true;
                }

                if (node["markercolor"])
                    rhs.markercolor(florb::color(node["markercolor"].as<unsigned int>()));
                
                if (node["markercolorselected"])
                    rhs.markercolorselected(florb::color(node["markercolorselected"].as<unsigned int>()));

                if (node["trackcolor"])
                    rhs.trackcolor(florb::color(node["trackcolor"].as<unsigned int>()));

                if (node["selectioncolor"])
                    rhs.selectioncolor(florb::color(node["selectioncolor"].as<unsigned int>()));

                if (node["gpscursorcolor"])
                    rhs.gpscursorcolor(florb::color(node["gpscursorcolor"].as<unsigned int>()));
                
                if (node["tracklinewidth"])
                    rhs.tracklinewidth(node["tracklinewidth"].as<unsigned int>());
                
                return true;
            }
        };

    template<>
        struct convert<florb::cfg_viewport> {
            static Node encode(const florb::cfg_viewport& rhs) {
                Node node;
                node["lon"] = rhs.lon();
                node["lat"] = rhs.lat();
                node["z"] = rhs.z();
                return node;
            }

            static bool decode(const Node& node, florb::cfg_viewport& rhs) 
            {
                if((!node.IsMap()) || (node.size() == 0))
                {
                    return true;
                }


                if (node["lat"])
                    rhs.lat(node["lat"].as<double>());

                if (node["lon"])
                    rhs.lon(node["lon"].as<double>());

                if (node["z"])
                    rhs.z(node["z"].as<unsigned int>());
                
                return true;
            }
        };
}

class florb::yaml_node
{
    public:
        yaml_node(YAML::Node n) : m_node(n) {};
        yaml_node(const std::string& path)
        {
            // Touch cfgfile in case it doesn't exist
            florb::utils::touch(path);
            m_node = YAML::LoadFile(path);
        }
        ~yaml_node() {};

        YAML::Node& get() { return m_node; };
    private:
        YAML::Node m_node;  
};

class florb::yaml_iterator
{
    public:
        yaml_iterator(YAML::iterator i) : m_iter(i) {};
        ~yaml_iterator() {};

        YAML::iterator& get() { return m_iter; };
    private:
        YAML::iterator m_iter;  
};

florb::node::iterator::iterator(const node& n, int be) 
{
    if (be <= 0)
        m_ref = new yaml_iterator(n.m_ref->get().begin());
    else
        m_ref = new yaml_iterator(n.m_ref->get().end());
}

florb::node::iterator::~iterator()
{   
    delete m_ref;
}

bool florb::node::iterator::operator==(iterator const& rhs) const 
{
    return (m_ref->get() == rhs.m_ref->get());
}

bool florb::node::iterator::operator!=(iterator const& rhs) const 
{
    return !(m_ref->get() == rhs.m_ref->get());
}

florb::node::iterator& florb::node::iterator::operator++() 
{
    (m_ref->get())++;
    return *this;
}   

florb::node::iterator florb::node::iterator::operator++(int) 
{
    florb::node::iterator tmp (*this);
    ++(*this);
    return tmp;
}

// Bidirectional iterators are not supported by YAML-Cpp
#if 0
iterator& florb::node::iterator::operator--() 
{
    (m_ref.get())--;
    return *this;
}

iterator florb::node::iterator::operator--(int) 
{
    iterator tmp (*this);
    --(*this);
    return tmp;
}
#endif

florb::node florb::node::iterator::operator* () const 
{
    florb::yaml_node *tmp = new florb::yaml_node(*(m_ref->get()));
    return florb::node(tmp);
}

florb::settings::settings()
{
    m_cfgfile = florb::utils::appdir() + florb::utils::pathsep() + "config";
    florb::utils::mkdir(florb::utils::appdir());

    m_rootnode = florb::node(m_cfgfile);
    defaults(m_cfgfile);
}

florb::settings::~settings()
{
    m_rootnode.serialize(m_cfgfile);
}

florb::settings& florb::settings::get_instance()
{
    static florb::settings instance;
    return instance;
}

void florb::settings::defaults(const std::string& path)
{
    // Tileserver default configuration
    florb::cfg_tileserver cfgtileserver;
    if((!m_rootnode["tileservers"]) || (!m_rootnode["tileservers"].is_sequence()))
    {
        std::vector<florb::cfg_tileserver> v;
        v.push_back(cfgtileserver);

        m_rootnode["tileservers"] = v;
    }

    // GPSd default configuration
    florb::cfg_gpsd cfggpsd = m_rootnode["gpsd"].as<florb::cfg_gpsd>();
    m_rootnode["gpsd"] = cfggpsd;

    // Cache default configuration
    florb::cfg_cache cfgcache = m_rootnode["cache"].as<florb::cfg_cache>();
    m_rootnode["cache"] = cfgcache;

    // UI default configuration
    florb::cfg_ui cfgui = m_rootnode["ui"].as<florb::cfg_ui>();
    m_rootnode["ui"] = cfgui;

    // Viewport default configuration
    florb::cfg_viewport cfgvp = m_rootnode["viewport"].as<florb::cfg_viewport>();
    m_rootnode["viewport"] = cfgvp;

    // Units default configuration
    florb::cfg_units cfgunits = m_rootnode["units"].as<florb::cfg_units>();
    m_rootnode["units"] = cfgunits;
}

florb::node::node(const std::string& path)
{
    m_ref = new florb::yaml_node(path);
}

florb::node::node(const node& n)
{
    m_ref = new florb::yaml_node(n.m_ref->get());
}

florb::node::node()
{
    m_ref = new florb::yaml_node(YAML::Node());
}

florb::node::~node()
{
    if (m_ref)
        delete m_ref;
}

bool florb::node::is_sequence()
{
    return m_ref->get().IsSequence();
}

florb::node florb::node::operator[] (const int idx)
{
    return florb::node(new florb::yaml_node(m_ref->get()[idx]));
}

florb::node florb::node::operator[] (const std::string &name)
{
    return florb::node(new florb::yaml_node(m_ref->get()[name]));
}

florb::node& florb::node::operator= (const node& n)
{
    if (m_ref)
        delete m_ref;

    m_ref = new florb::yaml_node(n.m_ref->get());

    return *this;
}

template<typename T> florb::node& florb::node::operator= (const T& rhs)
{
    m_ref->get() = rhs;
    return *this;
}

florb::node::operator bool() const 
{
    return m_ref->get();
}

template<typename T> T florb::node::as() const
{
    return m_ref->get().as<T>();
}

template<typename T> void florb::node::push_back(const T& rhs)
{
    m_ref->get().push_back<T>(rhs);
}

void florb::node::push_back(const node& rhs)
{
    m_ref->get().push_back(rhs.m_ref->get());
}

void florb::node::serialize(const std::string& path)
{
    std::ofstream fout(path.c_str(), std::fstream::in|std::fstream::out|std::fstream::trunc);
    fout << m_ref->get();
}

size_t florb::node::size()
{
    return m_ref->get().size();
}

// This is necessary so YAML::Node template internals remain hidden from the
// header file
template int florb::node::as<int>() const;
template std::string florb::node::as<std::string>() const;
template florb::cfg_tileserver florb::node::as<florb::cfg_tileserver>() const;
template std::vector< florb::cfg_tileserver > florb::node::as< std::vector<florb::cfg_tileserver> >() const;
template bool florb::node::as<bool>() const;
template florb::cfg_cache florb::node::as<florb::cfg_cache>() const;
template florb::cfg_gpsd florb::node::as<florb::cfg_gpsd>() const;
template florb::cfg_ui florb::node::as<florb::cfg_ui>() const;
template florb::cfg_viewport florb::node::as<florb::cfg_viewport>() const;
template florb::cfg_units florb::node::as<florb::cfg_units>() const;

template void florb::node::push_back<int>(const int& rhs);
template void florb::node::push_back<std::string>(const std::string& rhs);
template void florb::node::push_back<florb::cfg_tileserver>(const florb::cfg_tileserver& rhs);

template florb::node& florb::node::operator=<int> (const int& rhs);
template florb::node& florb::node::operator=< std::vector<int> > (const std::vector<int>& rhs);
template florb::node& florb::node::operator=<std::string> (const std::string& rhs);
template florb::node& florb::node::operator=< std::vector<florb::cfg_tileserver> > (const std::vector<florb::cfg_tileserver>& rhs);
template florb::node& florb::node::operator=<florb::cfg_tileserver> (const florb::cfg_tileserver& rhs);
template florb::node& florb::node::operator=<bool> (const bool& rhs);
template florb::node& florb::node::operator=<florb::cfg_gpsd> (const florb::cfg_gpsd& rhs);
template florb::node& florb::node::operator=<florb::cfg_ui> (const florb::cfg_ui& rhs);
template florb::node& florb::node::operator=<florb::cfg_cache> (const florb::cfg_cache& rhs);
template florb::node& florb::node::operator=<florb::cfg_viewport> (const florb::cfg_viewport& rhs);
template florb::node& florb::node::operator=<florb::cfg_units> (const florb::cfg_units& rhs);

