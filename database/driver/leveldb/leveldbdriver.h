#ifndef LEVELDBDATABASE_H
#define LEVELDBDATABASE_H

#include "abstractdriver.h"

namespace leveldb {
class DB;  // forward declaration
}

namespace krdb {

class LevelDbDriver: public AbstractDriver
{
public:
    LevelDbDriver(QObject *parent = nullptr);
    virtual ~LevelDbDriver();
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

private:
    leveldb::DB* db_;
};

} // namespace

#endif
