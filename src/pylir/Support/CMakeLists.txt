# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(PylirSupport Text.cpp TextData.cpp BigInt.cpp)
target_link_libraries(PylirSupport PRIVATE utf8proc PUBLIC tommath)
if (PYLIR_USE_PCH)
    target_link_libraries(PylirSupport PRIVATE COMMON_PCH)
endif ()
