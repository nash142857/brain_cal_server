// Copyright (c) Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//! A component that stores the program version.

#ifndef QSVERSION_H
#define QSVERSION_H

#include <QHash>
class QString;
class QDebug;

class QsVersion
{
public:
   QsVersion();
   QsVersion(int major, int minor, int release = 0);
   explicit QsVersion(const QString& versionString);

   //! returns the minor version number
   int minorVersion() const;
   //! returns the major version number
   int majorVersion() const;
   //! returns the release number
   int releaseVersion() const;
   //! returns the version number as "major.minor.release"
   QString toString() const;
   //! checks that the version is not 0.0.0
   bool isValid() const;

   bool operator!=(const QsVersion& other) const;
   bool operator==(const QsVersion& other) const;
   bool operator>(const QsVersion& other) const;
   bool operator<(const QsVersion& other) const;

private:
   int mMajor;
   int mMinor;
   int mRelease;
};

inline uint qHash(const QsVersion &key)
{
    return qHash(key.majorVersion() ^ key.minorVersion());
}

QDebug operator<<(QDebug dbg, const QsVersion &version);

#endif // QSVERSION_H
