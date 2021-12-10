/*
 * Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ecclesia/lib/redfish/devpath.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "ecclesia/lib/redfish/interface.h"
#include "ecclesia/lib/redfish/node_topology.h"
#include "ecclesia/lib/redfish/property_definitions.h"

namespace libredfish {

std::optional<std::string> GetDevpathForUri(const NodeTopology &topology,
                                            absl::string_view uri) {
  auto it = topology.uri_to_associated_node_map.find(uri);
  if (it != topology.uri_to_associated_node_map.end() && !it->second.empty()) {
    // Assume that first Node will represent the local devpath for the URI
    return it->second.front()->local_devpath;
  }
  return std::nullopt;
}

std::optional<std::string> GetSensorDevpathFromNodeTopology(
    RedfishObject* obj, const NodeTopology& topology) {
  auto related_uri = (*obj)[kRfPropertyRelatedItem][0].AsObject();
  if (related_uri != nullptr && related_uri->GetUri().has_value()) {
    auto related_devpath = GetDevpathForUri(topology, *related_uri->GetUri());
    if (related_devpath.has_value()) {
      return *related_devpath;
    }
  }
  // Fallback to providing devpath for obj
  auto sensor_uri = obj->GetUri();
  if (sensor_uri.has_value()) {
    auto sensor_devpath = GetDevpathForUri(topology, *sensor_uri);
    if (sensor_devpath.has_value()) {
      return *sensor_devpath;
    }

    // If sensor itself and Related Item don't have devpaths, check the prefix
    // Chassis URI
    std::vector<absl::string_view> uri_parts = absl::StrSplit(*sensor_uri, '/');
    // The URI should be of the format
    // /redfish/v1/Chassis/<chassis-id>/{Thermals#|Sensors|Power#}/...
    // So we can resize the parts down to /redfish/v1/Chassis/<chassis-id> which
    // is equivalent to five parts: "", "redfish", "v1", "Chassis",
    // "<chassis-id>"
    if (uri_parts.size() > 5 && uri_parts[1] == kRfPropertyRedfish &&
        uri_parts[2] == kRfPropertyV1 && uri_parts[3] == kRfPropertyChassis) {
      uri_parts.resize(5);
      auto devpath = GetDevpathForUri(topology, absl::StrJoin(uri_parts, "/"));
      if (devpath.has_value()) {
        return *devpath;
      }
    }
  }
  return std::nullopt;
}

}  // namespace libredfish