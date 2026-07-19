#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "RepositoryData.h"

class DataStorage
{
public:
    explicit DataStorage(const string& filePath = "data/database.txt");

    bool saveAll(const RepositoryData& data) const;

    bool loadAll(RepositoryData& data) const;

private:
    string m_filePath;
};

#endif