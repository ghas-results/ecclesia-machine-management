licenses(["notice"])

cc_library(
    name = "interface",
    hdrs = ["interface.h"],
    visibility = [
        "//ecclesia/lib/redfish:__subpackages__",
        "//platforms/gbmc/mtest:__subpackages__",
        "//platforms/redfish/lib/query_engine:__subpackages__",
    ],
    deps = [
        "//ecclesia/lib/redfish:interface",
        "//ecclesia/lib/redfish:node_topology",
        "//ecclesia/lib/redfish/dellicius/query:query_cc_proto",
        "//ecclesia/lib/redfish/dellicius/query:query_result_cc_proto",
        "//ecclesia/lib/status:macros",
        "//ecclesia/lib/time:clock",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_library(
    name = "normalizer",
    srcs = ["normalizer.cc"],
    hdrs = ["normalizer.h"],
    visibility = [
        "//ecclesia/lib/redfish/dellicius:__subpackages__",
        "//platforms/gsys/daemon/redfish_server:__subpackages__",
    ],
    deps = [
        ":interface",
        "//ecclesia/lib/redfish:devpath",
        "//ecclesia/lib/redfish:interface",
        "//ecclesia/lib/redfish:node_topology",
        "//ecclesia/lib/redfish/dellicius/query:query_cc_proto",
        "//ecclesia/lib/redfish/dellicius/query:query_result_cc_proto",
        "//ecclesia/lib/redfish/dellicius/utils:id_assigner",
        "//ecclesia/lib/redfish/dellicius/utils:path_util",
        "//ecclesia/lib/time:proto",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "query_planner",
    srcs = ["query_planner.cc"],
    hdrs = ["query_planner.h"],
    visibility = [
        "//ecclesia/lib/redfish/dellicius:__subpackages__",
        "//platforms/redfish/lib/query_engine:__subpackages__",
    ],
    deps = [
        ":interface",
        "//ecclesia/lib/redfish:interface",
        "//ecclesia/lib/redfish/dellicius/query:query_cc_proto",
        "//ecclesia/lib/redfish/dellicius/query:query_result_cc_proto",
        "//ecclesia/lib/redfish/dellicius/utils:path_util",
        "//ecclesia/lib/status:macros",
        "@com_google_absl//absl/container:btree",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/log",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_googleapis//google/rpc:code_cc_proto",
        "@com_google_googleapis//google/rpc:status_cc_proto",
        "@com_googlesource_code_re2//:re2",
    ],
)

cc_library(
    name = "passkey",
    hdrs = ["passkey.h"],
    # We tightly control this visibility for the passkey factory. New clients using the raw RedfishInterface pointer need to add themselves to the redfish_query_engine_raw_intf_users package group.
    visibility = ["//ecclesia:redfish_query_engine_raw_intf_users"],
)
