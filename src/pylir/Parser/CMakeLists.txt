# Copyright 2022 Markus Böck
#
# Licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(Parser
        ParserExpressions.cpp
        Dumper.cpp
        ParserStatements.cpp
        Syntax.cpp
        ParserCompounds.cpp
        Visitor.cpp
        Parser.cpp)
target_link_libraries(Parser PUBLIC Lexer)
if (PYLIR_USE_PCH)
    target_link_libraries(Parser PRIVATE COMMON_PCH)
endif ()
