#ifndef ABSTRACT_REPOSITORY_H
#define ABSTRACT_REPOSITORY_H

#include <optional>

template <typename T>
class AbstractRepository
{
public:

    virtual ~AbstractRepository() = default;

    virtual int save(const T& item) = 0;

    virtual bool remove(int id) = 0;

    virtual std::optional<T> search(int id) const = 0;
};

#endif // ABSTRACT_REPOSITORY_H
