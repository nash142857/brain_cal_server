// Copyright (c), Razvan Petru
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

#ifndef QSLANGUAGE_H
#define QSLANGUAGE_H

#include <QList>
#include <QString>
#include <QHash>
class QTranslator;

class LanguageItem
{
public:
   QString name;
   QString shortName;
   int id;
};
typedef QList<LanguageItem> LanguageList;

class QsLanguage
{
public:
   QsLanguage();
   ~QsLanguage();
   const LanguageList& languages() const;
   const LanguageItem& defaultLanguage() const;
   //! returns the index of the language with the given id or -1
   int indexOf(int id) const;
   //! returns the index of the language with the given name or -1
   int indexOfName(const QString& name) const;
   //! returns the index of the language with the given name or -1
   int indexOfShortName(const QString& sname) const;

   const LanguageItem& activeLanguage() const;
   bool saveActiveLanguage(int langId);
   bool setApplicationLanguage(const int newLanguageId);

private:
   LanguageList mLanguages;
   QHash<int,QTranslator*> mLangIdToTranslator;
   QHash<int,QTranslator*> mLangIdToQtTranslator;
   int mApplicationLanguage;
};

#endif // QSLANGUAGE_H
