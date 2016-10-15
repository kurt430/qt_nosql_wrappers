#ifndef LMDBDRIVER_H
#define LMDBDRIVER_H

#include "abstractdriver.h"

extern "C"
{
struct MDB_env;  // forward declaration
struct MDB_txn;
struct MDB_cursor;
typedef unsigned int MDB_dbi;
}

namespace krdb {

class LmDbDriver: public AbstractDriver
{
public:
    LmDbDriver(QObject *parent = nullptr);
    virtual ~LmDbDriver();

    static const QString engineId();
    QString engineVersionName() const override;

    bool open(const QString &path) override;
    bool close() override;

    bool put(const Slice& key, const Slice& value) override;
    QByteArray get(const Slice& key) override;
    bool remove(const Slice &key) override;

    bool begin() override;
    bool commit() override;
    bool rollback() override;

    QString errorText() const override;

protected:
    void closeTransaction(bool abort);
    bool prepareActiveTransaction(bool readOnly);

private:
   MDB_env *env_{nullptr};
   bool trxnReadOnly_;
   MDB_txn *transaction_{nullptr};
   MDB_dbi  dbi_{0};
   int rc_;
};

} // namespace

#endif // LMDBDRIVER_H
