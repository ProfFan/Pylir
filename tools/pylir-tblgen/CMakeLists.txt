# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_tablegen(pylir-tblgen PYLIR main.cpp IntrinsicGen.cpp)
target_link_libraries(pylir-tblgen PRIVATE MLIRTblgenLib MLIRSupportIndentedOstream)
