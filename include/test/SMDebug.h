#pragma once

// Use this header to to debugging tasks that cannot be done by breaking
// execution flow (breakpoints) and manual tracing. This can be useful
// for tracing multithreading bugs which can only be reproduced by keeping
// the application running and through repetition. Also useful for IDE's
// that are not debugging-friendly (e.g. Xcode).
// Use the SMDBG macro as a non-breaking breakpoint (!?), i.e, rather than
// breaking the execution, SMDBG will just print the thread ID, file name
// and line number, and the function name (wherever it occurs) to the
// standard output. This will help you see if a certain point in code is
// executed. To output debugging info to qDebug() instead of a stream, use
// the SMQDBG macro instead of SMDBG and SMQWCH instead of SMWCH.
// There is also the possibility to add runtime "expression watches" using
// the macro SMWCH. Just pass the expression to SMWCH and then
// output it to SMDBG (or any ostream) using the left-shift operator.
// Use
// SMDW1(x);
// SMDW2(x, y);
// SMWD3(x, y, z);
// ...
// as a short-hand for
// SMDBG << SMWCH(x);
// SMDBG << SMWCH(x) << SMWCH(y);
// SMDBG << SMWCH(x) << SMWCH(y) << SMWCH(z);
// ...
// qDebug() equivalents for the short-hand macros are SMQWD1, ...
// Defining the following macros changes the behaviour and determines what
// details each SMDBG will send to the output:
#if 0
#define SM_DEBUG_THREADS // Should it behave in multithreaded environment
#define SM_DEBUG_FILE_LINENUM // Should it output filename and line number
#define SM_DEBUG_FUNCTION // Should it output the function name
#define SM_DEBUG_OSTREAM std::cout // The output stream to capture debug info
#define SM_DEBUG_OUTFILE "SMDebug.log" // The output file instead of stream
#define SM_DISABLE_SMQDBG // Disable inclusion and use of qDebug()
#endif
// To write the debug output to a file instead of the standard output, define
// the SM_DEBUG_OUTFILE as the full path to the file.
// After running the program where these macros are used, you can inspect
// the output to trace issues.
// Furthermore, in non-debug mode (if the macro NDEBUG is defined), all
// usages of these macros reduce to almost no-op. However, it is a best
// practice to make sure that your debugging code (including asserts,
// qDebug, etc) doesn't make its way to the production repository as they
// reduce code readability and might not be useful to other developers.
// To disable SMQDBG and definitions related to Qt, define SM_DISABLE_SMQDBG.
// This is useful when using this header in a project that does not include
// Qt headers.
// Here is a demonstration of the usage of this header:
#if 0

// We want everything printed (this could be done using compiler command line)
// Defining these does not enable the debugging macros in non-debug mode.
#define SM_DEBUG_THREADS
#define SM_DEBUG_FILE_LINENUM
#define SM_DEBUG_FUNCTION

#include "SMDebug.h"

void usage()
{
    int a = 1, b = 2;

    // SMDBG - Thd 1 - SMDebug.h:36 - usage():
    SMDBG;

    // SMDBG - Thd 1 - SMDebug.h:42 - usage():
    //     a = 1 - Thd 1
    //     b = 2 - Thd 1
    //     a + b = 3 - Thd 1
    SMDBG << SMWCH(a) << SMWCH(b) << SMWCH(a + b);
}

#endif

#include <iostream>
#include <sstream>

#ifndef NDEBUG
#define SM_DEBUG_ENABLED
#endif // NDEBUG

#ifdef SM_DEBUG_OUTFILE
#include <fstream>
#undef SM_DEBUG_OSTREAM
#define SM_DEBUG_OSTREAM outFile_
#else // SM_DEBUG_OSTREAM
#ifndef SM_DEBUG_OSTREAM
#define SM_DEBUG_OSTREAM std::cout
#endif // SM_DEBUG_OSTREAM
#endif // SM_DEBUG_OUTFILE

#ifndef SM_DISABLE_SMQDBG
#include <QDebug>
#ifdef SM_DEBUG_QTHREADS
#ifndef SM_DEBUG_THREADS
#define SM_DEBUG_THREADS
#endif // SM_DEBUG_THREADS
#include <QThread>
#define SM_DEBUG_OUTPUT_QTHREAD_ID " - Thd " << QThread::currentThreadId() << ""
#else
#define SM_DEBUG_OUTPUT_QTHREAD_ID
#endif // SM_DEBUG_QTHREADS
#endif // SM_DISABLE_SMQDBG

#ifdef SM_DEBUG_THREADS
#include <mutex>
#include <thread>
#define SM_DEBUG_OUTPUT_THREAD_ID " - Thd " << std::this_thread::get_id() << ""
#else
#define SM_DEBUG_OUTPUT_THREAD_ID ""
#endif // SM_DEBUG_THREADS

#ifdef SM_DEBUG_FILE_LINENUM
#define SM_DEBUG_OUTPUT_FILE_LINE_NUM " - " << \
    (SMDebug::Stringify(__FILE__) + ":" + SMDebug::Stringify(__LINE__)).c_str()\
    << ""
#else
#define SM_DEBUG_OUTPUT_FILE_LINE_NUM ""
#endif // SM_DEBUG_FILE_LINENUM

#ifdef SM_DEBUG_FUNCTION
#define SM_DEBUG_OUTPUT_FUNCTION " - in " << \
    (std::string("\"") + SMDebug::Stringify(__func__) + "\"").c_str() << ""
#else
#define SM_DEBUG_OUTPUT_FUNCTION ""
#endif // SM_DEBUG_FILE_LINENUM

#define SMDBG SMDebug::Instance() << "SMDBG" SM_DEBUG_OUTPUT_THREAD_ID \
    SM_DEBUG_OUTPUT_FILE_LINE_NUM SM_DEBUG_OUTPUT_FUNCTION ":\n"

#ifndef SM_DISABLE_SMQDBG
#define SMQDBG SMDebug::qInstance() << "SMQDBG" SM_DEBUG_OUTPUT_QTHREAD_ID \
    SM_DEBUG_OUTPUT_FILE_LINE_NUM SM_DEBUG_OUTPUT_FUNCTION ":\n"
#endif // SM_DISABLE_SMQDBG

#define SMWCH(EXPR) \
    "    " #EXPR " = " << (EXPR) << SM_DEBUG_OUTPUT_THREAD_ID "\n"

#ifndef SM_DISABLE_SMQDBG
#define SMQWCH(EXPR) \
    "    " #EXPR " = " << (EXPR) << SM_DEBUG_OUTPUT_QTHREAD_ID "\n"
#endif // SM_DISABLE_SMQDBG

#define SMDW1(X1) SMDBG SMWCH(X1)
#define SMDW2(X1, X2) SMDW1(X1) SMWCH(X2)
#define SMDW3(X1, X2, X3) SMDW2(X1, X2) SMWCH(X3)
#define SMDW4(X1, X2, X3, X4) SMDW3(X1, X2, X3) SMWCH(X4)
#define SMDW5(X1, X2, X3, X4, X5) SMDW4(X1, X2, X3, X4) SMWCH(X5)
#define SMDW6(X1, X2, X3, X4, X5, X6) SMDW5(X1, X2, X3, X4, X5) SMWCH(X6)

#ifndef SM_DISABLE_SMQDBG
#define SMQDW1(X1) SMQDBG SMQWCH(X1)
#define SMQDW2(X1, X2) SMQDW1(X1) SMQWCH(X2)
#define SMQDW3(X1, X2, X3) SMQDW2(X1, X2) SMQWCH(X3)
#define SMQDW4(X1, X2, X3, X4) SMQDW3(X1, X2, X3) SMQWCH(X4)
#define SMQDW5(X1, X2, X3, X4, X5) SMQDW4(X1, X2, X3, X4) SMQWCH(X5)
#define SMQDW6(X1, X2, X3, X4, X5, X6) SMQDW5(X1, X2, X3, X4, X5) SMQWCH(X6)
#endif // SM_DISABLE_SMQDBG

class SMDebug
{
public:
    SMDebug()
#ifdef SM_DEBUG_OUTFILE
        : outFile_(SM_DEBUG_OUTFILE)
#endif // SM_DEBUG_OUTFILE
    {
    }

    template<typename T>
    static std::string Stringify(T&& val)
    {
        std::ostringstream str;
        str << val;
        return str.str();
    }

    template<typename T>
    inline SMDebug& operator<<(T&& value)
    {
#ifdef SM_DEBUG_ENABLED
#ifdef SM_DEBUG_THREADS
        std::lock_guard<std::mutex> lock(outputMutex_);
#endif // SM_DEBUG_THREADS
        SM_DEBUG_OSTREAM << value;
#endif // SM_DEBUG_ENABLED
#ifdef SM_DEBUG_OUTFILE
        outFile_.flush();
#endif // SM_DEBUG_OUTFILE

        return *this;
    }

#ifndef SM_DISABLE_SMQDBG
    inline static QDebug qInstance()
    {
#if defined(SM_DEBUG_ENABLED) && defined(SM_DEBUG_OUTFILE)
        static bool qDebugRedirected = false;
        if (!qDebugRedirected)
        {
            qInstallMessageHandler(SMDebug::CustomQDebugMsgHandler);
            qDebugRedirected = true;
        }
#endif // defined(SM_DEBUG_ENABLED) && defined(SM_DEBUG_OUTFILE)
        return qDebug();
    }
#endif // SM_DISABLE_SMQDBG

#if defined(SM_DEBUG_OUTFILE) && !defined(SM_DISABLE_SMQDBG)
    static void CustomQDebugMsgHandler(
        QtMsgType, const QMessageLogContext &, const QString &msg)
    {
        auto localMsg = msg.toStdString();
        Instance().outFile_ << localMsg << std::endl;
    }
#endif // defined(SM_DEBUG_OUTFILE) && !defined(SM_DISABLE_SMQDBG)

    inline static SMDebug& Instance()
    {
        static SMDebug instance;
        return instance;
    }

private:
#ifdef SM_DEBUG_THREADS
    std::mutex outputMutex_;
#endif // SM_DEBUG_THREADS
#ifdef SM_DEBUG_OUTFILE
    std::ofstream outFile_;
#endif // SM_DEBUG_OUTFILE
};
