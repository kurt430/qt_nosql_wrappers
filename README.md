**Simple Qt wrappers (only key-value storage)**

# Supported libraries:
- [LevelDB](https://github.com/google/leveldb/) ([BSD-3-Clause](https://github.com/google/leveldb/blob/master/LICENSE))
- [Vedis](http://vedis.symisc.net/) ([GPL compatible license](http://opensource.org/licenses/Sleepycat))
- [UnQLite](https://unqlite.org/) ([BSD license](http://opensource.org/licenses/BSD-2-Clause))
- [LMDB](https://symas.com/products/lightning-memory-mapped-database/) ([OpenLDAP Public License](http://www.openldap.org/software/release/license.html))

# Basic usage:
```C++
#include "abstractdriver.h"
#include "driverfactory.h"

using namespace krdb;

AbstractDriver *db = DriverFactory::create("vedis", this); // "leveldb", "unqlite", "lmdb"
if (db && db->open("test.db")) {
    db->put("key", "value");
    if (db->get("key") != "value") {
        // ...
    }
    db->close();
}
```
# Primitive benchmarks: 
*Platform: Windows, Qt 5.6.2(mingw), i3 2.7GHz, no SSD*

| Library/test               | LevelDB v1.19 | Vedis v1.2.6 | UnQLite v1.1.6 | LMDB v0.9.70 |
| -------------------------- | -------------:| ------------:| --------------:| ------------:|
| Write 1Mb block, ms        |           5.1 |          115 |             61 |         0.11 |
| Write 10k short blocks, ms |            56 |    0.0000073 |      0.0000073 |    0.0000073 |
| Read 10k short blocks, ms  |            52 |    0.0000082 |      0.0000076 |    0.0000073 |
| Storage file size, bytes   |       9102018 |      3866624 |        3866624 |      2048000 |
