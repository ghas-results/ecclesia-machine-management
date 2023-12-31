/*
 * Copyright 2022 Google LLC
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

#ifndef ECCLESIA_LIB_REDFISH_DELLICIUS_ENGINE_QUERY_ENGINE_H_
#define ECCLESIA_LIB_REDFISH_DELLICIUS_ENGINE_QUERY_ENGINE_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "absl/base/attributes.h"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "absl/types/span.h"
#include "ecclesia/lib/file/cc_embed_interface.h"
#include "ecclesia/lib/redfish/dellicius/engine/config.h"
#include "ecclesia/lib/redfish/dellicius/engine/factory.h"
#include "ecclesia/lib/redfish/dellicius/engine/internal/interface.h"
#include "ecclesia/lib/redfish/dellicius/engine/internal/passkey.h"
#include "ecclesia/lib/redfish/dellicius/query/query_result.pb.h"
#include "ecclesia/lib/redfish/dellicius/utils/id_assigner.h"
#include "ecclesia/lib/redfish/interface.h"
#include "ecclesia/lib/redfish/node_topology.h"
#include "ecclesia/lib/redfish/topology.h"
#include "ecclesia/lib/redfish/transport/cache.h"
#include "ecclesia/lib/redfish/transport/http_redfish_intf.h"
#include "ecclesia/lib/redfish/transport/interface.h"
#include "ecclesia/lib/redfish/transport/transport_metrics.pb.h"
#include "ecclesia/lib/time/clock.h"

namespace ecclesia {

// QueryEngine is logical composition of Redfish query interpreter, dispatcher
// and normalizer built to execute a statically defined set of Redfish Queries
// based on accompanying optional query rules.
// A few ways to instantiate QueryEngine using factory APIs:
//  (A) Build QueryEngine with default configuration (local id based on redfish
//      stable id):
//  QueryContext query_context{.query_files = query_files};
//  absl::StatusOr<QueryEngine> query_engine = CreateQueryEngine(query_context,
//      {.transport = std::move(transport)});
//
//  (B) Build QueryEngine with non default local stable id:
//  QueryContext query_context{.query_files = query_files};
//  absl::StatusOr<QueryEngine> query_engine = CreateQueryEngine(query_context,
//      {.transport = std::move(transport),
//       .stable_id_type =
//            QueryEngineParams::RedfishStableIdType::kRedfishLocationDerived});
//
//  (C) Build QueryEngine with machine level stable id decorator:
//  QueryContext query_context{.query_files = query_files};
//  absl::StatusOr<QueryEngine> query_engine =
//    CreateQueryEngine<MyStableIdMapType>(
//        query_context,
//        {.transport = std::move(transport),
//          .entity_tag = "node0",
//          .stable_id_type =
//              QueryEngineParams::RedfishStableIdType::kRedfishLocation},
//        std::move(my_stable_id_map), std::move(my_machine_id_assigner));
//
//  (D) Build QueryEngine using custom normalizer:
//  QueryContext query_context{.query_files = query_files};
//  absl::StatusOr<QueryEngine> query_engine = CreateQueryEngine(
//     query_context, std::move(redfish_interface),
//     std::move(my_custom_normalizer));
class QueryEngine {
 public:
  enum class ServiceRootType { kRedfish, kGoogle };

  // Interface for private implementation of Query Engine using PImpl Idiom
  class QueryEngineIntf {
   public:
    virtual ~QueryEngineIntf() = default;
    virtual std::vector<DelliciusQueryResult> ExecuteQuery(
        ServiceRootType service_root_uri,
        absl::Span<const absl::string_view> query_ids) = 0;
    virtual std::vector<DelliciusQueryResult> ExecuteQuery(
        ServiceRootType service_root_uri,
        absl::Span<const absl::string_view> query_ids,
        QueryTracker &tracker) = 0;
    virtual std::vector<DelliciusQueryResult> ExecuteQueryWithMetrics(
        ServiceRootType service_root_uri,
        absl::Span<const absl::string_view> query_ids,
        RedfishMetrics *transport_metrics) = 0;
    virtual const NodeTopology &GetTopology() = 0;
    // QueryEngineRawInterfacePasskey is just an empty strongly-typed object
    // that one needs to provide in order to invoke the member function.
    // We restrict the visibility of QueryEngineRawInterfacePasskey so that
    // we can understand which users are using raw-interface features which
    // are not yet available in the query engine.
    virtual absl::StatusOr<RedfishInterface *> GetRedfishInterface(
        RedfishInterfacePasskey unused_passkey) = 0;
  };

  ABSL_DEPRECATED("Use QueryEngine factory methods instead.")
  QueryEngine(const QueryEngineConfiguration &config,
              std::unique_ptr<RedfishTransport> transport,
              RedfishTransportCacheFactory cache_factory = NullCache::Create,
              const Clock *clock = Clock::RealClock());

  explicit QueryEngine(std::unique_ptr<QueryEngineIntf> engine_impl)
      : engine_impl_(std::move(engine_impl)) {}

  QueryEngine(const QueryEngine &) = delete;
  QueryEngine &operator=(const QueryEngine &) = delete;
  QueryEngine(QueryEngine &&other) = default;
  QueryEngine &operator=(QueryEngine &&other) = default;

  std::vector<DelliciusQueryResult> ExecuteQuery(
      absl::Span<const absl::string_view> query_ids,
      ServiceRootType service_root_uri = ServiceRootType::kRedfish) {
    return engine_impl_->ExecuteQuery(service_root_uri, query_ids);
  }
  std::vector<DelliciusQueryResult> ExecuteQuery(
      absl::Span<const absl::string_view> query_ids, QueryTracker &tracker,
      ServiceRootType service_root_uri = ServiceRootType::kRedfish) {
    return engine_impl_->ExecuteQuery(service_root_uri, query_ids, tracker);
  }
  // Transport metrics flag must be true for metrics to be populated.
  std::vector<DelliciusQueryResult> ExecuteQueryWithMetrics(
      absl::Span<const absl::string_view> query_ids,
      RedfishMetrics *transport_metrics,
      ServiceRootType service_root_uri = ServiceRootType::kRedfish) {
    return engine_impl_->ExecuteQueryWithMetrics(service_root_uri, query_ids,
                                                 transport_metrics);
  }
  const NodeTopology &GetTopology() { return engine_impl_->GetTopology(); }
  absl::StatusOr<RedfishInterface *> GetRedfishInterface(
      RedfishInterfacePasskey unused_passkey) {
    return engine_impl_->GetRedfishInterface(unused_passkey);
  }

 private:
  std::unique_ptr<QueryEngineIntf> engine_impl_;
};

// Encapsulates the context needed to execute RedPath query.
struct QueryContext {
  // Describes the RedPath queries that engine will be configured to execute.
  absl::Span<const EmbeddedFile> query_files;
  // Rules used to configure Redfish query parameter - $expand for
  // specific RedPath prefixes in given queries.
  absl::Span<const EmbeddedFile> query_rules = {};
  const Clock *clock = Clock::RealClock();
};

// Parameters necessary to configure the query engine.
struct QueryEngineParams {
  // Stable id types used to configure engine for an appropriate normalizer that
  // decorates the query result with desired stable
  // id type.
  enum class RedfishStableIdType {
    kRedfishLocation,  // Redfish Standard - PartLocationContext + ServiceLabel
    kRedfishLocationDerived  // Derived from Redfish topology.
  };

  // Transport medium over which Redfish queries are sent to the redfish server.
  std::unique_ptr<RedfishTransport> transport;
  // Generates cache used by query engine, default set to Null cache (no cache).
  RedfishTransportCacheFactory cache_factory = NullCache::Create;
  // Optional attribute to uniquely identify redfish server where necessary.
  std::string entity_tag;
  // Type of stable identifier to use in query result
  QueryEngineParams::RedfishStableIdType stable_id_type =
      QueryEngineParams::RedfishStableIdType::kRedfishLocation;
};

inline std::unique_ptr<Normalizer> BuildLocalDevpathNormalizer(
    QueryEngineParams::RedfishStableIdType stable_id_type,
    RedfishInterface *redfish_interface) {
  switch (stable_id_type) {
    case QueryEngineParams::RedfishStableIdType::kRedfishLocation:
      return BuildDefaultNormalizer();
    case QueryEngineParams::RedfishStableIdType::kRedfishLocationDerived:
      return BuildDefaultNormalizerWithLocalDevpath(
          CreateTopologyFromRedfish(redfish_interface));
  }
}

template <typename LocalIdMapT>
std::unique_ptr<Normalizer> BuildMachineDevpathNormalizer(
    const std::string &server_tag,
    QueryEngineParams::RedfishStableIdType stable_id_type,
    std::unique_ptr<LocalIdMapT> local_id_map,
    const IdAssignerFactory<LocalIdMapT> &id_assigner_factory,
    RedfishInterface *redfish_interface) {
  switch (stable_id_type) {
    case QueryEngineParams::RedfishStableIdType::kRedfishLocation:
      return BuildDefaultNormalizerWithMachineDevpath<LocalIdMapT>(
          server_tag, std::move(local_id_map), id_assigner_factory);
    case QueryEngineParams::RedfishStableIdType::kRedfishLocationDerived:
      return BuildDefaultNormalizerWithMachineDevpath<LocalIdMapT>(
          server_tag, std::move(local_id_map), id_assigner_factory,
          CreateTopologyFromRedfish(redfish_interface));
  }
}

// Creates query engine to execute queries in given |query_context| over the
// |redfish_interface| provided.
// Caller can optionally provide a |normalizer| for the queried data.
absl::StatusOr<QueryEngine> CreateQueryEngine(
    const QueryContext &query_context,
    std::unique_ptr<RedfishInterface> redfish_interface,
    std::unique_ptr<Normalizer> normalizer = BuildDefaultNormalizer());

// Build query engine based on given |configuration| to execute queries in
// |query_context|.
absl::StatusOr<QueryEngine> CreateQueryEngine(const QueryContext &query_context,
                                              QueryEngineParams configuration);

// Creates query engine for machine devpath DecoratorExtensions.
template <typename LocalIdMapT>
absl::StatusOr<QueryEngine> CreateQueryEngine(
    const QueryContext &query_context, QueryEngineParams engine_params,
    std::unique_ptr<LocalIdMapT> local_id_map,
    const IdAssignerFactory<LocalIdMapT> &id_assigner_factory) {
  // Build Redfish interface
  std::unique_ptr<RedfishInterface> redfish_interface = NewHttpInterface(
      std::move(engine_params.transport),
      std::move(engine_params.cache_factory), RedfishInterface::kTrusted);

  if (redfish_interface == nullptr)
    return absl::InternalError("Can't create redfish interface");
  std::unique_ptr<Normalizer> normalizer = BuildMachineDevpathNormalizer(
      engine_params.entity_tag, engine_params.stable_id_type,
      std::move(local_id_map), id_assigner_factory, redfish_interface.get());

  return CreateQueryEngine(query_context, std::move(redfish_interface),
                           std::move(normalizer));
}

}  // namespace ecclesia

#endif  // ECCLESIA_LIB_REDFISH_DELLICIUS_ENGINE_QUERY_ENGINE_H_
