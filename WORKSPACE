workspace(name = "com_google_ecclesia")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Google Test. Official release 1.10.0.
http_archive(
    name = "com_google_googletest",
    sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
    strip_prefix = "googletest-release-1.10.0",
    urls = ["https://github.com/google/googletest/archive/release-1.10.0.tar.gz"],
)

# Google benchmark. Official release 1.5.6.
http_archive(
    name = "com_github_google_benchmark",
    sha256 = "789f85b4810d13ff803834ea75999e41b326405d83d6a538baf01499eda96102",
    strip_prefix = "benchmark-1.5.6",
    urls = ["https://github.com/google/benchmark/archive/refs/tags/v1.5.6.tar.gz"],
)

# Abseil. Latest feature not releases yet. Picked up a commit from Sep 2, 2020
http_archive(
    name = "com_google_absl",
    sha256 = "5ec35586b685eea11f198bb6e75f870e37fde62d15b95a3897c37b2d0bbd9017",
    strip_prefix = "abseil-cpp-143a27800eb35f4568b9be51647726281916aac9",
    urls = ["https://github.com/abseil/abseil-cpp/archive/143a27800eb35f4568b9be51647726281916aac9.zip"],
)

# Emboss. Uses the latest commit as of May 19, 2021.
http_archive(
    name = "com_google_emboss",
    sha256 = "53971feb699d35cd96986cf451eb85986974d65429807c3c2b168c6786846b34",
    strip_prefix = "emboss-5cb347f85c9f1d2b7d00c29bd08ef706d8cd0461",
    urls = ["https://github.com/google/emboss/archive/5cb347f85c9f1d2b7d00c29bd08ef706d8cd0461.tar.gz"],
)

# Protocol buffers. Official release 3.17.0.
http_archive(
    name = "com_google_protobuf",
    sha256 = "eaba1dd133ac5167e8b08bc3268b2d33c6e9f2dcb14ec0f97f3d3eed9b395863",
    strip_prefix = "protobuf-3.17.0",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.17.0.tar.gz"],
)

# Google APIs. Latest commit as of Nov 16, 2020.
http_archive(
    name = "com_google_googleapis",
    sha256 = "979859a238e6626850fee33d30f4240e90e71009786a55a15134df582dbc2dbe",
    strip_prefix = "googleapis-8d245ac97e058b541f1477b1a85d676b18e80849",
    urls = ["https://github.com/googleapis/googleapis/archive/8d245ac97e058b541f1477b1a85d676b18e80849.tar.gz"],
)

# Google APIs imports. Required to build googleapis.
load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,
    grpc = True,
)

# Needs to come before grpc_deps() to be respected.
http_archive(
    name = "boringssl",
    sha256 = "66e1b0675d58b35f9fe3224b26381a6d707c3293eeee359c813b4859a6446714",
    strip_prefix = "boringssl-9b7498d5aba71e545747d65dc65a4d4424477ff0",
    urls = [
        "https://github.com/google/boringssl/archive/9b7498d5aba71e545747d65dc65a4d4424477ff0.tar.gz",
    ],
)

# gRPC. Taken from HEAD to include compiler fix for gcc error. Name is required
# by Google APIs.
http_archive(
    name = "com_github_grpc_grpc",
    patches = [
        "//ecclesia/oss:grpc.patches/grpc.visibility.patch",
        "//ecclesia/oss:grpc.patches/grpc.delete_ios.patch",
    ],
    patch_args = ["-p1"],
    sha256 = "ca12845fd97777caa2277de31b80c59f2f777bde1e86e116bd21e5e0598c48d4",
    strip_prefix = "grpc-74d0e3905e9d5a94f592813cb1f137fb60a907b8",
    urls = ["https://github.com/grpc/grpc/archive/74d0e3905e9d5a94f592813cb1f137fb60a907b8.tar.gz"],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

# Skylib libraries.
http_archive(
    name = "bazel_skylib",
    sha256 = "ea5dc9a1d51b861d27608ad3bd6c177bc88d54e946cb883e9163e53c607a9b4c",
    strip_prefix = "bazel-skylib-2b38b2f8bd4b8603d610cfc651fcbb299498147f",
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/2b38b2f8bd4b8603d610cfc651fcbb299498147f.tar.gz"],
)

# Extra build rules for various languages.
http_archive(
    name = "rules_python",
    sha256 = "e5470e92a18aa51830db99a4d9c492cc613761d5bdb7131c04bd92b9834380f6",
    strip_prefix = "rules_python-4b84ad270387a7c439ebdccfd530e2339601ef27",
    urls = ["https://github.com/bazelbuild/rules_python/archive/4b84ad270387a7c439ebdccfd530e2339601ef27.tar.gz"],
)

http_archive(
    name = "rules_pkg",
    sha256 = "b9d1387deed06eef45edd3eb7fd166577b8ad1884cb6a17898d136059d03933c",
    strip_prefix = "rules_pkg-0.2.6-1/pkg",
    urls = ["https://github.com/bazelbuild/rules_pkg/archive/0.2.6-1.tar.gz"],
)

http_archive(
    # Needed for gRPC.
    name = "build_bazel_rules_swift",
    sha256 = "d0833bc6dad817a367936a5f902a0c11318160b5e80a20ece35fb85a5675c886",
    strip_prefix = "rules_swift-3eeeb53cebda55b349d64c9fc144e18c5f7c0eb8",
    urls = ["https://github.com/bazelbuild/rules_swift/archive/3eeeb53cebda55b349d64c9fc144e18c5f7c0eb8.tar.gz"],
)

# Build rules for boost, which will fetch all of the boost libs. They're then
# available under @boost//, e.g. @boost//:graph for the graph library.
http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "0a1d884aa13201b705f93b86d2d0be1de867f6e592de3c4a3bbe6d04bdddf593",
    strip_prefix = "rules_boost-a32cad61d9166d28ed86d0e07c0d9bca8db9cb82",
    urls = ["https://github.com/nelhage/rules_boost/archive/a32cad61d9166d28ed86d0e07c0d9bca8db9cb82.tar.gz"],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()

# Subpar, an external equivalent for building .par files.
http_archive(
    name = "subpar",
    sha256 = "b80297a1b8d38027a86836dbadc22f55dc3ecad56728175381aa6330705ac10f",
    strip_prefix = "subpar-2.0.0",
    urls = ["https://github.com/google/subpar/archive/2.0.0.tar.gz"],
)

# TensorFlow depends on "io_bazel_rules_closure" so we need this here.
# Needs to be kept in sync with the same target in TensorFlow's WORKSPACE file.
http_archive(
    name = "io_bazel_rules_closure",
    sha256 = "9b99615f73aa574a2947226c6034a6f7c319e1e42905abc4dc30ddbbb16f4a31",
    strip_prefix = "rules_closure-4a79cc6e6eea5e272fe615db7c98beb8cf8e7eb5",
    urls = [
        "http://mirror.tensorflow.org/github.com/bazelbuild/rules_closure/archive/4a79cc6e6eea5e272fe615db7c98beb8cf8e7eb5.tar.gz",
        "https://github.com/bazelbuild/rules_closure/archive/4a79cc6e6eea5e272fe615db7c98beb8cf8e7eb5.tar.gz",  # 2019-09-16
    ],
)

http_archive(
    name = "com_github_libevent_libevent",
    build_file = "@//ecclesia/oss:libevent.BUILD",
    sha256 = "8836ad722ab211de41cb82fe098911986604f6286f67d10dfb2b6787bf418f49",
    strip_prefix = "libevent-release-2.1.12-stable",
    urls = [
        "https://github.com/libevent/libevent/archive/release-2.1.12-stable.zip",
    ],
)

http_archive(
    name = "com_googlesource_code_re2",
    sha256 = "26155e050b10b5969e986dab35654247a3b1b295e0532880b5a9c13c0a700ceb",
    strip_prefix = "re2-2021-06-01",
    urls = [
        "https://github.com/google/re2/archive/refs/tags/2021-06-01.tar.gz",
    ],
)

http_archive(
    name = "org_tensorflow",
    # NOTE: when updating this, MAKE SURE to also update the protobuf_js runtime version
    # in third_party/workspace.bzl to >= the protobuf/protoc version provided by TF.
    sha256 = "48ddba718da76df56fd4c48b4bbf4f97f254ba269ec4be67f783684c75563ef8",
    strip_prefix = "tensorflow-2.0.0-rc0",
    urls = [
        "http://mirror.tensorflow.org/github.com/tensorflow/tensorflow/archive/v2.0.0-rc0.tar.gz",  # 2019-08-23
        "https://github.com/tensorflow/tensorflow/archive/v2.0.0-rc0.tar.gz",
    ],
)

#tensorflow. Commit from June 23, 2021 making HTTP server support PATCH
http_archive(
    name = "com_google_tensorflow_serving",
    patches = [
        "//ecclesia/oss:tensorflow.patches/tensorflow.visibility.patch",
    ],
    sha256 = "9635a59a23981bb61661b94059fd10f8365b3f316212b0eb5c5c9ffb8be911b6",
    strip_prefix = "serving-6cbc4a9eb419c8078c3a4e791381cda70dd8fc78",
    urls = ["https://github.com/tensorflow/serving/archive/6cbc4a9eb419c8078c3a4e791381cda70dd8fc78.zip"],
)

load("@com_google_tensorflow_serving//tensorflow_serving:workspace.bzl", "tf_serving_workspace")

tf_serving_workspace()

# JSON for Modern C++ version 3.9.1.
http_archive(
    name = "com_json",
    build_file = "@//ecclesia/oss:json.BUILD",
    sha256 = "4cf0df69731494668bdd6460ed8cb269b68de9c19ad8c27abc24cd72605b2d5b",
    strip_prefix = "json-3.9.1",
    urls = ["https://github.com/nlohmann/json/archive/v3.9.1.tar.gz"],
)

http_archive(
    name = "zlib",
    build_file = "@com_google_protobuf//:third_party/zlib.BUILD",
    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    strip_prefix = "zlib-1.2.11",
    urls = ["https://zlib.net/zlib-1.2.11.tar.gz"],
)

http_archive(
    name = "ncurses",
    build_file = "@//ecclesia/oss:ncurses.BUILD",
    sha256 = "30306e0c76e0f9f1f0de987cf1c82a5c21e1ce6568b9227f7da5b71cbea86c9d",
    strip_prefix = "ncurses-6.2",
    urls = [
        "http://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.2.tar.gz",
    ],
)

http_archive(
    name = "libedit",
    build_file = "@//ecclesia/oss:libedit.BUILD",
    sha256 = "dbb82cb7e116a5f8025d35ef5b4f7d4a3cdd0a3909a146a39112095a2d229071",
    strip_prefix = "libedit-20191231-3.1",
    urls = [
        "https://www.thrysoee.dk/editline/libedit-20191231-3.1.tar.gz",
    ],
)

http_archive(
    name = "ipmitool",
    build_file = "@//ecclesia/oss:ipmitool.BUILD",
    patch_cmds = [
        "./bootstrap",
        "./configure CFLAGS=-fPIC CXXFLAGS=-fPIC --enable-shared=no",
        "cp ./config.h include",
    ],
    patches = [
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_ipmi_intf.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.src_ipmitool.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_helper.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_ipmi_sel.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.lib_ipmi_sel.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.src_plugins_ipmi_intf.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.src_plugins_lanplus_lanplus.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.src_ipmiext.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.lib_ipmi_main.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_ipmi_sdr.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.lib_ipmi_raw.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_ipmi.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.lib_ipmi_sdr.c.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.include_ipmitool_ipmi_user.h.patch",
        "//ecclesia/oss:ipmitool.patches/ipmitool.lib_ipmi_user.c.patch",
    ],
    sha256 = "c8549064def9c38acd8d3d9bf976952e792b714206285c9c4b9ff6c9c56a17fc",
    strip_prefix = "ipmitool-c3939dac2c060651361fc71516806f9ab8c38901",
    urls = [
        "https://github.com/ipmitool/ipmitool/archive/c3939dac2c060651361fc71516806f9ab8c38901.tar.gz",
    ],
)

http_archive(
    name = "curl",
    build_file = "@//ecclesia/oss:curl.BUILD",
    sha256 = "01ae0c123dee45b01bbaef94c0bc00ed2aec89cb2ee0fd598e0d302a6b5e0a98",
    strip_prefix = "curl-7.69.1",
    urls = [
        "https://storage.googleapis.com/mirror.tensorflow.org/curl.haxx.se/download/curl-7.69.1.tar.gz",
        "https://curl.haxx.se/download/curl-7.69.1.tar.gz",
    ],
)

http_archive(
    name = "jansson",
    build_file = "@//ecclesia/oss:jansson.BUILD",
    sha256 = "5f8dec765048efac5d919aded51b26a32a05397ea207aa769ff6b53c7027d2c9",
    strip_prefix = "jansson-2.12",
    urls = [
        "https://digip.org/jansson/releases/jansson-2.12.tar.gz",
    ],
)

http_archive(
    name = "redfishMockupServer",
    build_file = "@//ecclesia/oss:redfishMockupServer.BUILD",
    patches = [
      "//ecclesia/oss:redfishMockupServer.patches/0001-googlify-import-and-add-ipv6-support.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0002-logger-level-to-critical.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0003-patch-dir-traversal-vulnerability.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0004-add-uds-support.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0005-support-payload-post.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0006-Reply-payload-and-token-headers-if-post-to-Sessions.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0007-Add-mTLS-support.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0008-Add-EventService-support.patch",
      "//ecclesia/oss:redfishMockupServer.patches/0009-add-link-local-support.patch",
    ],
    patch_args = ["-p1"],
    sha256 = "63a144428b0bdb0203ed302c6e9d58fba9dac5e1e399bc6a49cbb30c14b05c23",
    strip_prefix = "Redfish-Mockup-Server-04238cb8b7b8d8d8a62cac6623e2c355a6b73eb8",
    urls = ["https://github.com/DMTF/Redfish-Mockup-Server/archive/04238cb8b7b8d8d8a62cac6623e2c355a6b73eb8.tar.gz"],
)

http_archive(
    name = "libsodium",
    build_file = "@//ecclesia/oss:libsodium.BUILD",
    patches = [
        "//ecclesia/oss:libsodium.patches/libsodium.01.version_h.patch",
    ],
    sha256 = "d59323c6b712a1519a5daf710b68f5e7fde57040845ffec53850911f10a5d4f4",
    strip_prefix = "libsodium-1.0.18",
    urls = ["https://github.com/jedisct1/libsodium/archive/1.0.18.tar.gz"],
)

http_archive(
    name = "zeromq",
    build_file = "@//ecclesia/oss:zeromq.BUILD",
    patches = [
        "//ecclesia/oss:zeromq.patches/zmq.01.add_platform_hpp.patch",
    ],
    sha256 = "27d1e82a099228ee85a7ddb2260f40830212402c605a4a10b5e5498a7e0e9d03",
    strip_prefix = "zeromq-4.2.1",
    urls = ["https://github.com/zeromq/libzmq/releases/download/v4.2.1/zeromq-4.2.1.tar.gz"],
)

http_archive(
    name = "cppzmq",
    build_file = "@//ecclesia/oss:cppzmq.BUILD",
    sha256 = "9853e0437d834cbed5d3c223bf1d755cadee70e7c964c6e42c4c6783dee5d02c",
    strip_prefix = "cppzmq-4.7.1",
    urls = ["https://github.com/zeromq/cppzmq/archive/v4.7.1.tar.gz"],
)

# Riegeli. Uses the latest commit as of Mar 17, 2021.
http_archive(
    name = "com_google_riegeli",
    sha256 = "8f28ca19b1ebe96df6c1d76ecadf1aa4e7fcf151c0492e91b7401a47ce2add62",
    strip_prefix = "riegeli-9c3f3203ad04a45fe8743bb71cd0cd98c76e394d",
    url = "https://github.com/google/riegeli/archive/9c3f3203ad04a45fe8743bb71cd0cd98c76e394d.tar.gz",
)

# Additional projects needed by riegeli.
http_archive(
    name = "org_brotli",
    sha256 = "6e69be238ff61cef589a3fa88da11b649c7ff7a5932cb12d1e6251c8c2e17a2f",
    strip_prefix = "brotli-1.0.7",
    urls = ["https://github.com/google/brotli/archive/v1.0.7.zip"],
)

http_archive(
    name = "net_zstd",
    build_file = "@com_google_riegeli//third_party:net_zstd.BUILD",
    sha256 = "b6c537b53356a3af3ca3e621457751fa9a6ba96daf3aebb3526ae0f610863532",
    strip_prefix = "zstd-1.4.5/lib",
    urls = ["https://github.com/facebook/zstd/archive/v1.4.5.zip"],
)

http_archive(
    name = "highwayhash",
    build_file = "@com_google_riegeli//third_party:highwayhash.BUILD",
    sha256 = "cf891e024699c82aabce528a024adbe16e529f2b4e57f954455e0bf53efae585",
    strip_prefix = "highwayhash-276dd7b4b6d330e4734b756e97ccfb1b69cc2e12",
    urls = ["https://github.com/google/highwayhash/archive/276dd7b4b6d330e4734b756e97ccfb1b69cc2e12.zip"],
)

http_archive(
    name = "snappy",
    build_file = "@com_google_riegeli//third_party:snappy.BUILD",
    sha256 = "e170ce0def2c71d0403f5cda61d6e2743373f9480124bcfcd0fa9b3299d428d9",
    strip_prefix = "snappy-1.1.9",
    urls = ["https://github.com/google/snappy/archive/1.1.9.zip"],
)
