#ifndef UNQLITEDRIVER_H
#define UNQLITEDRIVER_H

#include "abstractdriver.h"

extern "C" {
struct unqlite; // forward declaration
struct unqlite_kv_cursor;
}

namespace krdb {

class UnqliteDriver: public AbstractDriver
{
public:
    UnqliteDriver(QObject *parent = nullptr);
    virtual ~UnqliteDriver();
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
    unqlite *db_;
    int rc_;
};

} // namespace

#endif
