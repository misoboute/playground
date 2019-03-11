#include <iostream>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QMap>
#include <QString>

#define SM_DEBUG_FILE_LINENUM // Should it output filename and line number
#include "SMDebug.h"

QString CleanNonAlphabeticChars(const QString& alphabet, const QString& input)
{
    QString output;
    for (auto ch : input)
    {
        if (alphabet.contains(ch))
        {
            output += ch;
        }
        else if (QString("ؤئإأ").contains(ch))
        {
            output += "ء";
        }
        else if (QString("ي").contains(ch))
        {
            output += "ی";
        }
        else if (QString("ة").contains(ch))
        {
            output += "ه";
        }
        else if (QString("آ").contains(ch))
        {
            output += "ا";
        }
    }
    return output;
}

QString Encrypt(QString clearText, QString rhyme, 
    const std::vector<int>& metre, const QString& alphabet)
{
    const auto alphabetSize = alphabet.size();
    auto letterIndex = QMap<QChar, int>();
    for (auto i = 0; i < alphabetSize; ++i)
    {
        letterIndex[alphabet[i]] = i;
    }

    rhyme = CleanNonAlphabeticChars(alphabet, rhyme);
    clearText = CleanNonAlphabeticChars(alphabet, clearText);
    auto encrypted = QString();
    auto counter = 0;
    const auto rhymeSize = rhyme.size();
    const auto metreSize = metre.size();
    for (const auto letter : clearText)
    {
        auto skipCount = metre[counter % metreSize];
        auto keyLetter = rhyme[counter % rhymeSize];
        auto keyValue = letterIndex[keyLetter];
        auto letterValue = letterIndex[letter];
        auto encryptedLetterValue = (letterValue + keyValue) % alphabetSize;
        auto encryptedLetter = alphabet[encryptedLetterValue];
        for (auto i = 0; i < skipCount; ++i)
        {
            encrypted += "?, ";
        }
        encrypted += encryptedLetter;
        encrypted += (++counter) % metreSize == 0 ? "\n" : ", ";
    }
    if (encrypted.endsWith(", "))
    {
        encrypted.truncate(encrypted.size() - 2);
    }
    return encrypted;
}

std::vector<int> ParseMeter(const QString& metreStr)
{
    auto metre = std::vector<int>(metreStr.size(), 0);
    for (auto i = 0; i < metre.size(); ++i)
    {
        auto value = metreStr[i].digitValue();
        if (value < 0)
        {
            throw std::runtime_error("Failed to parse metre string");
        }
        metre[i] = value;
    }
    return metre;
}

void TestAlphabet(const QString& alpha)
{
    const auto testLetters = QString("ؤئيإأآةءضصثقفغعهخحجچشسیبلاتنمکگظطزرذدپو");
    for (auto i = 0; i < testLetters.size(); ++i)
    {
        if (!alpha.contains(testLetters[i]))
        {
            std::cout << "Letter " << i << " missing from the alphabet." << 
                std::endl;
        }
    }
}

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        std::cerr << "Insufficient parameters:" << std::endl << 
            argv[0] << " <input-clear-text-file> <output-encrypted-file" << 
            " <rhyme> <metre>" << std::endl << 
            "rhyme should be a string and metre of the form 21222122212" <<
            " which is FA-E-LA-TON FA-E-LA-TON FA-E-LAT" << std::endl;
        return 1;
    }
    
    QFile input(QString::fromUtf8(argv[1]));
    if (!input.exists())
    {
        std::cerr << "Input file not found!" << std::endl;
        return 2;
    }

    QFile output(QString::fromUtf8(argv[2]));
    input.open(QIODevice::ReadOnly);
    output.open(QIODevice::WriteOnly);
    auto clearText = QString::fromUtf8(input.readAll());
    auto rhyme = QString::fromUtf8(argv[3]);
    auto metre = ParseMeter(QString::fromUtf8(argv[4]));
    const auto alphabet = QString("ءابپتثجچحخدذرزژسشصضطظعغفقکگلمنوهی");
    auto encrypted = Encrypt(clearText, rhyme, metre, alphabet);
    output.write(encrypted.toUtf8());
    return 0;
}
