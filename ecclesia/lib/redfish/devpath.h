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

#ifndef ECCLESIA_LIB_REDFISH_DEVPATH_H_
#define ECCLESIA_LIB_REDFISH_DEVPATH_H_

#include <optional>
#include <string>

#include "absl/strings/string_view.h"
#include "ecclesia/lib/redfish/interface.h"
#include "ecclesia/lib/redfish/node_topology.h"

namespace ecclesia {

// Function to find devpath for an arbitrary URI by searching through
// NodeTopology
std::optional<std::string> GetDevpathForUri(const NodeTopology& topology,
                                            absl::string_view uri);

// Function to find devpath for Sensor resources (Sensor/Power/Thermal) based on
// RelatedItems
//
// This function will use the provided NodeTopology to lookup the "RelatedItem"
// provided by the RedfishObject. If none exists then the function will try and
// find a devpath for the Sensor object itself in the topology. As a final
// fallback, the function will try and find the containing Chassis for these
// resources and provide that devpath.
//
// The RedfishObject passed to this function must be under a subURI of a Chassis
// in order to produce a valid depvath i.e. /redfish/v1/Chassis/<chassis-id>/...
std::optional<std::string> GetSensorDevpathFromNodeTopology(
    RedfishObject* obj, const NodeTopology& topology);

}  // namespace ecclesia

#endif  // ECCLESIA_LIB_REDFISH_DEVPATH_H_
