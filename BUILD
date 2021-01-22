package(
    default_visibility = ["//visibility:public"],
)

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
  name = "libfountain/",
  includes = [
    "include/fountain/",
  ],
  srcs = glob([
    "include/fountain//*.h",
    "src/*.cpp",
  ]),
  linkopts = ["-lpthread"],
  linkstatic = False,
  copts = ["-std=c++11"],
)

cc_binary(
  name = "sample_ps",
  includes = [
    "include",
  ],
  srcs = glob([
    "sample/pub_sub/*.cpp",
    "sample/pub_sub/*.h",
  ]),
  deps = [":libfountain/"],
  copts = ["-std=c++11"],
)

cc_binary(
  name = "sample_sc",
  includes = [
    "include",
  ],
  srcs = glob([
    "sample/srv_call/*.cpp",
    "sample/srv_call/*.h",
  ]),
  deps = [":libfountain/"],
  copts = ["-std=c++11"],
)
