#include <memory/MemoryUtils.h>
#include "llapi/utils/WinHelper.h"
#include <ModUtils/ModUtils.h>
#include "llapi/HookAPI.h"
namespace ll::memory {

    FuncPtr resolveSymbol(const char* symbol) { return dlsym_real(symbol); }

    FuncPtr resolveSignature(const char* signature) { return reinterpret_cast<FuncPtr>(ModUtils::FindSig(signature)); }

} // namespace ll::memory