#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>

#include "event_serialize.hpp"
#include <phosphor-logging/log.hpp>
#include "config.h"

// Register class version
// From cereal documentation;
// "This macro should be placed at global scope"
CEREAL_CLASS_VERSION(phosphor::events::Entry, CLASS_VERSION);

namespace phosphor
{
namespace events
{

using namespace phosphor::logging;

/** @brief Function required by Cereal to perform serialization.
 *  @tparam Archive - Cereal archive type (binary in our case).
 *  @param[in] archive       - reference to Cereal archive.
 *  @param[in] event       - const reference to event entry.
 *  @param[in] version - Class version that enables handling
 *                       a serialized data across code levels
 */
template<class Archive>
void save(Archive& archive, const Entry& event, const std::uint32_t version)
{
    archive(event.id(),
        event.timestamp(),
        event.message(),
        event.additionalData());

}

/** @brief Function required by Cereal to perform deserialization.
 *  @tparam Archive - Cereal archive type (binary in our case).
 *  @param[in] archive       - reference to Cereal archive.
 *  @param[in] event       - reference to event entry.
 *  @param[in] version - Class version that enables handling
 *                       a serialized data across code levels
 */
template<class Archive>
void load(Archive& archive, Entry& event, const std::uint32_t version)
{
    using namespace
        sdbusplus::xyz::openbmc_project::Logging::server;

    uint32_t id{};
    uint64_t timestamp{};
    std::string message{};
    std::vector<std::string> additionalData{};

    archive(id, timestamp, message, additionalData);

    event.id(id);
    event.timestamp(timestamp);
    event.message(message);
    event.additionalData(additionalData);
}

fs::path serialize(const Entry& event,
                   const std::string eventName,
                   const fs::path& dir)
{
    auto path = dir / eventName;
    fs::create_directories(path);
    path /=  std::to_string(event.id());
    std::ofstream os(path.c_str(), std::ios::binary);
    cereal::BinaryOutputArchive oarchive(os);
    oarchive(event);
    return path;
}

bool deserialize(const fs::path& path, Entry& event)
{
    try
    {
        if (fs::exists(path))
        {
            std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
            cereal::BinaryInputArchive iarchive(is);
            iarchive(event);
            return true;
        }
        return false;
    }
    catch(cereal::Exception& e)
    {
        log<level::ERR>(e.what());
        fs::remove(path);
        return false;
    }
    catch(const std::length_error& e)
    {
        // Running into: USCiLab/cereal#192
        // This may be indicating some other issue in the
        // way vector may have been used inside the logging.
        // possibly associations ??. But handling it here for
        // now since we are anyway tossing the log
        // TODO: openbmc/phosphor-logging#8
        log<level::ERR>(e.what());
        fs::remove(path);
        return false;
    }
}

} // namespace event
} // namespace phosphor
