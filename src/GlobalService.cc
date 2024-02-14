#include "Global.h"
#include <GMLIB/include_ll.h>
#include <GMLIB/include_mc.h>

LL_AUTO_TYPE_INSTANCE_HOOK(
    DBStorageInitEvent,
    ll::memory::HookPriority::Normal,
    DBStorage,
    "??0DBStorage@@QEAA@UDBStorageConfig@@V?$not_null@V?$NonOwnerPointer@VLevelDbEnv@@@Bedrock@@@gsl@@@Z",
    DBStorage*,
    struct DBStorageConfig&                        a1,
    Bedrock::NotNullNonOwnerPtr<class LevelDbEnv>& a2
) {
    auto res                 = origin(a1, a2);
    GMLIB::Global<DBStorage> = this;
    return res;
}