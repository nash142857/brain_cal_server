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

#include "QsVersion.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>
#include <QtGlobal>

QsVersion::QsVersion(int major, int minor, int release) :
    mMajor(major),
    mMinor(minor),
    mRelease(release)
{
    Q_ASSERT(isValid());
}


//! Reads the version from the string. Sets it to 0.0 on failure.
QsVersion::QsVersion( const QString& a_versionString ) :
    mMajor(0),
    mMinor(0),
    mRelease(0)
{
    enum
    {
        MajorPart = 1,
        MinorPart,
        ReleasePart
    };
    const int partCount = 3;
    QRegExp versionRe("^(\\d{1,3})\\.(\\d{1,3})(?:\\.(\\d{1,4}))*$");
    if (a_versionString.contains(versionRe)) {
        mMajor = versionRe.cap(MajorPart).toInt();
        mMinor = versionRe.cap(MinorPart).toInt();
        if (partCount == versionRe.captureCount())
            mRelease = versionRe.cap(ReleasePart).toInt();
    }
}

QsVersion::QsVersion() :
    mMajor(0),
    mMinor(0),
    mRelease(0)
{
}

int QsVersion::minorVersion() const
{
    return mMinor;
}


int QsVersion::majorVersion() const
{
    return mMajor;
}


int QsVersion::releaseVersion() const
{
    return mRelease;
}

QString QsVersion::toString() const
{
    return QString("%1.%2.%3")
            .arg(mMajor)
            .arg(mMinor)
            .arg(mRelease);
}


bool QsVersion::isValid() const
{
    return mMajor || mMinor || mRelease;
}


// comparison operators
// --------------------
bool QsVersion::operator==(const QsVersion& other) const
{
    return mMajor == other.mMajor &&
            mMinor == other.mMinor &&
            mRelease == other.mRelease;
}

bool QsVersion::operator>(const QsVersion& other) const
{
    return mMajor > other.mMajor ||
            (mMajor == other.mMajor && mMinor > other.mMinor) ||
            (mMajor == other.mMajor && mMinor == other.mMinor && mRelease > other.mRelease);
}

bool QsVersion::operator<(const QsVersion& other) const
{
    return mMajor < other.mMajor ||
            (mMajor == other.mMajor && mMinor < other.mMinor) ||
            (mMajor == other.mMajor && mMinor == other.mMinor && mRelease < other.mRelease);
}

bool QsVersion::operator!=(const QsVersion& other) const
{
    return !(*this == other);
}

QDebug operator<<(QDebug dbg, const QsVersion &version)
{
    dbg.nospace() << version.toString();
    return dbg.space();
}
