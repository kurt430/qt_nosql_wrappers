#ifndef VEDISDRIVER_H
#define VEDISDRIVER_H

#include "abstractdriver.h"

extern "C" {
struct vedis; // forward declaration
}

namespace krdb {

class VedisDriver: public AbstractDriver
{
public:
    VedisDriver(QObject *parent = nullptr);
    virtual ~VedisDriver();
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

    virtual QString errorText() const override;

private:
    static int output_callback_to_existing_bytearray(const void *pOutput, unsigned int nLen, void *pUserData);

private:
    vedis *db_;
    int rc_;
};

} // namespace

#endif
