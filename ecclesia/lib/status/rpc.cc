/*
 * Copyright 2020 Google LLC
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

#include "ecclesia/lib/status/rpc.h"

#include <string>

#include "google/protobuf/any.pb.h"
#include "google/rpc/code.pb.h"
#include "google/rpc/status.pb.h"
#include "absl/status/status.h"
#include "absl/strings/cord.h"
#include "absl/strings/string_view.h"
#include "grpcpp/support/status.h"

namespace ecclesia {

google::rpc::Status StatusToRpcStatus(const absl::Status &status) {
  google::rpc::Status ret;
  ret.set_code(static_cast<int>(status.code()));
  ret.set_message(status.message().data(), status.message().size());
  status.ForEachPayload(
      [&](absl::string_view type_url, const absl::Cord &payload) {
        google::protobuf::Any *any = ret.add_details();
        any->set_type_url(std::string(type_url));
        *any->mutable_value() = std::string(payload);
      });
  return ret;
}

grpc::Status StatusToGrpcStatus(const absl::Status &status) {
  google::rpc::Status rpc_status = StatusToRpcStatus(status);
  std::string error_details;
  rpc_status.SerializeToString(&error_details);
  return grpc::Status(static_cast<grpc::StatusCode>(status.code()),
                      std::string(status.message()), std::move(error_details));
}

}  // namespace ecclesia
