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

#include "ecclesia/lib/redfish/proxy/redfish_proxy.h"

#include <memory>
#include <string>
#include <utility>

#include "ecclesia/lib/redfish/proto/redfish_v1.pb.h"
#include "grpcpp/client_context.h"
#include "grpcpp/server_context.h"
#include "grpcpp/support/status.h"

namespace ecclesia {
namespace {

using ::grpc::ClientContext;
using ::grpc::ServerContext;
using ::redfish::v1::Request;
using ::redfish::v1::Response;

// Forward metadata values from the client to the server.
void ForwardMetadataFromServerContext(ServerContext &server_context,
                                      ClientContext &client_context) {
  for (const auto &[key, value] : server_context.client_metadata()) {
    client_context.AddMetadata(std::string(key.data(), key.size()),
                               std::string(value.data(), value.size()));
  }
}

}  // namespace

// After authentication/authorization and forward the metadata from client to
// the server, all proxy RPCs will be implemented with handlers.
grpc::Status RedfishV1GrpcProxy::Get(ServerContext *context,
                                     const Request *request,
                                     Response *response) {
  if (IsRpcAuthorized(*context).code() != absl::StatusCode::kOk) {
    return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                        "User is not authorized to call this RPC");
  }
  auto client_context = ClientContext::FromServerContext(*context);
  ForwardMetadataFromServerContext(*context, *client_context);
  grpc::Status status = GetHandler(client_context.get(), request, response);
  return status;
}
grpc::Status RedfishV1GrpcProxy::Post(ServerContext *context,
                                      const Request *request,
                                      Response *response) {
  if (IsRpcAuthorized(*context).code() != absl::StatusCode::kOk) {
    return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                        "User is not authorized to call this RPC");
  }
  auto client_context = ClientContext::FromServerContext(*context);
  ForwardMetadataFromServerContext(*context, *client_context);
  grpc::Status status = PostHandler(client_context.get(), request, response);
  return status;
}
grpc::Status RedfishV1GrpcProxy::Put(ServerContext *context,
                                     const Request *request,
                                     Response *response) {
  if (IsRpcAuthorized(*context).code() != absl::StatusCode::kOk) {
    return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                        "User is not authorized to call this RPC");
  }
  auto client_context = ClientContext::FromServerContext(*context);
  ForwardMetadataFromServerContext(*context, *client_context);
  grpc::Status status = PutHandler(client_context.get(), request, response);
  return status;
}
grpc::Status RedfishV1GrpcProxy::Patch(ServerContext *context,
                                       const Request *request,
                                       Response *response) {
  if (IsRpcAuthorized(*context).code() != absl::StatusCode::kOk) {
    return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                        "User is not authorized to call this RPC");
  }
  auto client_context = ClientContext::FromServerContext(*context);
  ForwardMetadataFromServerContext(*context, *client_context);
  grpc::Status status = PatchHandler(client_context.get(), request, response);
  return status;
}
grpc::Status RedfishV1GrpcProxy::Delete(ServerContext *context,
                                        const Request *request,
                                        Response *response) {
  if (IsRpcAuthorized(*context) != absl::OkStatus()) {
    return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                        "User is not authorized to call this RPC");
  }
  auto client_context = ClientContext::FromServerContext(*context);
  ForwardMetadataFromServerContext(*context, *client_context);
  grpc::Status status = DeleteHandler(client_context.get(), request, response);
  return status;
}

}  // namespace ecclesia
