#pragma once

#include <tcb/span.hpp>

#include "Objects.hpp"

namespace pylir::rt
{

tcb::span<PyObject*> getHandles();

tcb::span<PyObject* const> getCollections();

bool isGlobal(PyObject* object);

} // namespace pylir::rt
