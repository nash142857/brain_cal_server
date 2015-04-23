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

#include "QsLanguage.h"
#include "QsLog.h"
#include <QObject>
#include <QSettings>
#include <QTranslator>
#include <QApplication>
#include <QScopedPointer>
#include <QtDebug>

QsLanguage::QsLanguage()
   : mApplicationLanguage(0)
{
   int id = 0;
   LanguageItem english;
   english.name = QObject::tr("English");
   english.shortName = "en";
   english.id = id++;
   mLanguages.push_back(english);

   LanguageItem romanian;
   romanian.name = QObject::tr("Romanian");
   romanian.shortName = "ro";
   romanian.id = id++;
   mLanguages.push_back(romanian);
}

QsLanguage::~QsLanguage()
{
   const int defaultLanguageId = defaultLanguage().id;
   if( mApplicationLanguage != defaultLanguageId )
   {
      Q_ASSERT_X(0,"QsLanguage","You forgot to uninstall the translators!");
      setApplicationLanguage(defaultLanguageId);
   }

   qDeleteAll(mLangIdToQtTranslator.values());
   qDeleteAll(mLangIdToTranslator.values());
}

const LanguageList& QsLanguage::languages() const
{
   return mLanguages;
}

const LanguageItem& QsLanguage::defaultLanguage() const
{
   return mLanguages.at(0);
}

int QsLanguage::indexOf(int id) const
{
   for(int i = 0, isize = mLanguages.size();i < isize;++i)
      if( mLanguages.at(i).id == id )
         return i;
   return -1;
}

int QsLanguage::indexOfName(const QString& name) const
{
   for(int i = 0, isize = mLanguages.size();i < isize;++i)
      if( mLanguages.at(i).name == name )
         return i;
   return -1;
}

int QsLanguage::indexOfShortName(const QString& sname) const
{
   for(int i = 0, isize = mLanguages.size();i < isize;++i)
      if( mLanguages.at(i).shortName == sname )
         return i;
   return -1;
}

//todo: cache
const LanguageItem& QsLanguage::activeLanguage() const
{
   const int activeLanguageId = QSettings().value(QLatin1String("language"),
      defaultLanguage().id).toInt();
   const int index = indexOf(activeLanguageId);
   if( -1 != index )
      return mLanguages.at(index);
   return defaultLanguage();
}

bool QsLanguage::saveActiveLanguage(int langId)
{
   QSettings settings;
   if( -1 == indexOf(langId) )
      return false;
   settings.setValue(QLatin1String("language"), langId);
   settings.sync();

   if( QSettings::NoError == settings.status() )
      return true;
   return false;
}

bool QsLanguage::setApplicationLanguage(const int newLanguageId)
{
   const int newLanguageIndex = indexOf(newLanguageId);
   if( -1 == newLanguageIndex )
      return false;

   const LanguageItem& newLanguage = mLanguages.at(newLanguageIndex);
   if( newLanguage.id == mApplicationLanguage )
      return true;

   // remove current translators
   if( mLangIdToTranslator.contains(mApplicationLanguage) )
      qApp->removeTranslator(mLangIdToTranslator.value(mApplicationLanguage, NULL));
   if( mLangIdToQtTranslator.contains(mApplicationLanguage) )
      qApp->removeTranslator(mLangIdToQtTranslator.value(mApplicationLanguage, NULL));

   if( newLanguageId == defaultLanguage().id )
   {
      mApplicationLanguage = newLanguageId;
      return true; // the default language doesn't need translators
   }

   const QString appDir = qApp->applicationDirPath();
   if( mLangIdToQtTranslator.contains(newLanguageId) )
      qApp->installTranslator(mLangIdToQtTranslator.value(newLanguageId, NULL));
   else // try to load from disk
   {
      QScopedPointer<QTranslator> t(new QTranslator);
      const bool loadedQt =
         t->load(QString("qt_%1.qm").arg(newLanguage.shortName),
         appDir);
      if( !loadedQt )
      {
         QLOG_WARN() << "Failed to load Qt translation for" << newLanguage.name;
         return false;
      }

      qApp->installTranslator(t.data());
      mLangIdToQtTranslator.insert(newLanguageId, t.take());
   }
   if( mLangIdToTranslator.contains(newLanguageId) )
      qApp->installTranslator(mLangIdToTranslator.value(newLanguageId, NULL));
   else // try to load from disk
   {
      QScopedPointer<QTranslator> t(new QTranslator);
      const bool loadedApp =
         t->load(QString("qswallet_%1.qm").arg(newLanguage.shortName),
         appDir);
      if( !loadedApp )
      {
         QLOG_WARN() << "Failed to load app translation for" << newLanguage.name;
         return false;
      }
      qApp->installTranslator(t.data());
      mLangIdToTranslator.insert(newLanguageId, t.take());
   }

   mApplicationLanguage = newLanguageId;
   return true;
}
