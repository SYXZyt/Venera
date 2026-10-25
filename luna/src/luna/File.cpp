#include "File.h"
#include <luna/Helpers.h>

Luna::File::File(std::string_view name, std::string_view contents) :
    mName(name),
    mContents(contents)
{
    mContents = std::move(Replace(mContents, "\t", "    "));
    mLines = Split(mContents, '\n');
}