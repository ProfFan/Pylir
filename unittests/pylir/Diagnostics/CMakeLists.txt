# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(Catch)

add_executable(diagnostics_tests main.cpp diagnostics_tests.cpp document_tests.cpp)
target_link_libraries(diagnostics_tests Diagnostics)
catch_discover_tests(diagnostics_tests)
