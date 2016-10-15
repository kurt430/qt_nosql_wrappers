#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

#include <QObject>

namespace krdb {

class Slice
{
public:
    const void *data_;
    size_t size_;

    Slice()  = default;
    ~Slice()  = default;
    Slice(Slice&& other)  = default;
    Slice& operator=(Slice&& other)  = default;

    // Create a slice that refers to data[0,size-1].
    Slice(const void* const data, size_t size)
        : data_(data), size_(size) {}

    // Create a slice that refers to s[0,strlen(s)-1]
    Slice(const char* s)
        : data_(s), size_(strlen(s)) {}

    // Create a slice that refers to the contents of "ba"
    Slice(const QByteArray& ba)
        : Slice(ba.constData(), static_cast<size_t>(ba.size())) {}

    // Create a slice that refers pointer to unsigned int
    Slice(unsigned int *pointer_to_uint)
        : data_(pointer_to_uint), size_(sizeof(unsigned int)) {}

    int size() const { return static_cast<int>(size_); }

    // Return a QByteArray that contains the copy of the referenced data.
    QByteArray toByteArray() const
    {
        return QByteArray(static_cast<const char *>(data_), static_cast<int>(size_));
    }

    unsigned int toUint() const
    {
        return *(static_cast<const unsigned int *>(data_));
    }

    // Return true iff "x" is a prefix of "*this"
    bool startsWith(const Slice& x) const
    {
      return ((size_ >= x.size_) && (memcmp(data_, x.data_, x.size_) == 0));
    }
};

static_assert(std::is_pod<Slice>::value, "Value must be a POD type");

class AbstractDriver: public QObject
{
public:
    AbstractDriver(QObject *parent = nullptr);

    virtual ~AbstractDriver();

    virtual QString engineVersionName() const = 0;

    virtual bool open(const QString &path) = 0;
    virtual bool close() = 0;

    virtual bool put(const Slice& key, const Slice& value) = 0;
    virtual QByteArray get(const Slice& key) = 0;
    virtual bool remove(const Slice &key) = 0;

    virtual bool begin() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;

    virtual QString errorText() const = 0;

private:
    AbstractDriver(const AbstractDriver&) = delete;
    AbstractDriver& operator=(const AbstractDriver&) = delete;
};

} // namespace

#endif //ABSTRACTDRIVER_H
